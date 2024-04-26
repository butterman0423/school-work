/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Macros for printing
#define BLUE 		"\x1b[34;1m"
#define DEFAULT 	"\x1b[0m"

// Macros for error strings
#define ERR_PSWD "Error: Cannot get passwd entry. %s.\n"
#define ERR_CDIR "Error: Cannot change directory to %s. %s.\n"
#define ERR_CARG "Error: Too many arguments to cd.\n"
#define ERR_SDIN "Error: Failed to read from stdin. %s.\n"
#define ERR_FORK "Error: fork() failed. %s.\n"
#define ERR_EXEC "Error: exec() failed. %s.\n"
#define ERR_WAIT "Error: wait() failed. %s.\n"
#define ERR_MLOC "Error: malloc() failed. %s.\n"
#define ERR_GCWD "Error: Cannot get current working directory. %s.\n"
#define ERR_SIGH "Error: Cannot register signal handler. %s.\n"

#define ERR_RLOC "Error: realloc() failed. %s.\n"
#define ERR_ODIR "Error: Failed to open directory. %s.\n"

#define ERRNO_MSG				strerror(errno)
#define REP_ERROR(msg, ...)		fprintf(stderr, msg, __VA_ARGS__)

// Macros for built-ins
#define BLT_CD		0x6463
#define BLT_EXIT	0x74697865
#define BLT_PWD		0x647770
#define BLT_LF		0x666c
#define BLT_LP		0x706c
#define BLT_EMPTY	0x0

// Misc
#define ENTRY_MAX	256
#define VECTOR_CAP	32

// Vector type
typedef struct {
	size_t _cap;
	size_t _size; 		// Item size
	
	void* items;
	size_t nsize;
} vector;

// Returns a new vector where each entry has <size> bytes
// NULL is returned if an error occured
vector* make_vector(size_t size) {
	vector* vec = (vector*) malloc(sizeof(vector));
	
	if(!vec) {
		REP_ERROR(ERR_MLOC, ERRNO_MSG);
		return NULL;
	}
	if( !(vec->items = malloc(size * VECTOR_CAP)) ) {
		free(vec);
		REP_ERROR(ERR_MLOC, ERRNO_MSG);
	}

	vec->_cap = VECTOR_CAP;
	vec->_size = size;
	vec->nsize = 0;
	return vec;
}

// Frees a vector
void destroy(vector* vec) {
	for(int i = 0; i < vec->nsize; i++) {
		void* p = vec->items + (i * vec->_size);
		free(*(char**) p);
	}
	free(vec->items);
	free(vec);
}

// Pushes an item into the vector
// NULL is returned and the passed
// vector is destroyed if an error occured
vector* push(vector* vec, void* item) {
	if(vec->nsize == vec->_cap) {
		vec->_cap <<= 1;
		void* new = realloc(vec->items, vec->_size * vec->_cap);
		
		if(!new) {
			REP_ERROR(ERR_RLOC, ERRNO_MSG);
			destroy(vec);
			return NULL;
		}

		vec->items = new;
	}

	void* pin = vec->items + (vec->nsize++ * vec->_size);
	
	if(!item) {
		*(char**) pin = NULL;
	}
	else {
		memcpy(pin, &item, vec->_size);
	}
	
	return vec;
}

// a and b are both char* that have a uid
int uid_cmpr(const void* a, const void* b) {
	unsigned int a_uid;
	unsigned int b_uid;

	sscanf( *(char**) a, "%u", &a_uid);
	sscanf( *(char**) b, "%u", &b_uid);

	return (int) (a_uid - b_uid);
}

// This function is intended to ignore
// the SIGINT signal
volatile int SIG_LOCK = 0;
struct sigaction def = {0};
void _(int sig) {
	printf("\n");
	SIG_LOCK = !SIG_LOCK;
	
	// Restore old behavior	for second SIGINT
	if( sigaction(SIGINT, &def, NULL) ) {
		REP_ERROR(ERR_SIGH, ERRNO_MSG);
	}		
}

// getcwd() wrapper
char* getcwd_wrap(char** dest) {
	char* prev = *dest;
	char* ret = getcwd(NULL, 0);

	if(!ret) {
		REP_ERROR(ERR_GCWD, ERRNO_MSG);
		return NULL;
	}

	free(prev);
	*dest = ret;
	return ret;
}

int main() {
	char buffer[BUFSIZ];
	long int blt_code;
	struct passwd* uinfo;
	struct dirent* dirp;
	DIR* dir;
	
	// Save default SIG_INT behavior
	sigaction(SIGINT, &def, &def);

	struct sigaction ignore = {0};
	ignore.sa_handler = &_;
	
	// Cwd is initially '\0'
	char* cwd = (char*) calloc(1, sizeof(char));
	getcwd_wrap(&cwd);
	
	while(1) {
		blt_code = 0;
		clearerr(stdin); // Clear stdin error flag
		
		// Prompt and input
		printf("%s[%s]%s>", BLUE, cwd, DEFAULT);
		fflush(stdout);
		
		fgets(buffer, BUFSIZ, stdin);
		if( ferror(stdin) ) {
			// Mute error if caused by SIGINT
			if( !SIG_LOCK ) { REP_ERROR(ERR_SDIN, ERRNO_MSG); }
			continue;
		}
	
		// Avoid a bad looping print dump after CTRL+D
		if( feof(stdin) ) {
			printf("\n");
			continue;
		}
		
		sscanf(buffer, "%5s", (char*) &blt_code);	// Voodo magic
		
		switch(blt_code) {

			case BLT_EMPTY:
				break;

			case BLT_EXIT:
				free(cwd);
				exit(EXIT_SUCCESS);
				break;

			case BLT_CD:
				char tmp;
				int count = sscanf(buffer, "%*s%s%1s", buffer, &tmp);
				
				if(count == 2) {
					fprintf(stderr, ERR_CARG);
					break;
				}
				
				// Set path to home if needed
				if( ((count == EOF || !strcmp(buffer, "~")) && !(buffer[0] = '\0')) || \
						sscanf(buffer, "~%s", buffer) ) {
					
					uid_t uid = getuid();
						
					if( !(uinfo = getpwuid(uid)) ) {
						REP_ERROR(ERR_PSWD, ERRNO_MSG);
						break;		
					}

					char tmp_buf[BUFSIZ];
					strcpy(tmp_buf, buffer);
					sprintf(buffer, "%s%s", uinfo->pw_dir, tmp_buf);
				}
				
				// Actually change directory
				if( chdir(buffer) ) {
					REP_ERROR(ERR_CDIR, buffer, ERRNO_MSG);
					break;
				}
				getcwd_wrap(&cwd);

				break;

			case BLT_PWD:
				char* tmp_cwd = NULL;
				// Just to set ERRNO in case of overwrite
				if( getcwd_wrap(&tmp_cwd) ) {
					printf("%s\n", tmp_cwd);
					free(tmp_cwd);
				}
				
				break;
			
			case BLT_LF:
				dir = opendir(".");
				
				if(!dir) { 
					REP_ERROR(ERR_ODIR, ERRNO_MSG);
					break;   
				}

				while( (dirp = readdir(dir)) ) {
					if( !strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, "..") ) {
						continue;
					}
					printf("%s\n", dirp->d_name);
				}

				closedir(dir);
				
				break;
			
			case BLT_LP:
				unsigned int pid;
				int max_len = 0;	// PID num width

				vector* entries = make_vector(sizeof(char*));
				if(!entries) { break; }

				dir = opendir("/proc");
				
				if(1 || !dir) { 
					REP_ERROR(ERR_ODIR, ERRNO_MSG);
					destroy(entries);
					break;   
				}

				while( (dirp = readdir(dir)) ) {
					if( !sscanf(dirp->d_name, "%u", &pid) ) {
						continue;
					}
					
					// Max PID num
					int len = strlen(dirp->d_name);
					max_len = len > max_len ? len : max_len;
					
					char* entry = (char*) malloc(sizeof(char) * ENTRY_MAX);
					if(!entry) { REP_ERROR(ERR_MLOC, ERRNO_MSG); destroy(entries); break; }

					sprintf(entry, "%u ", pid);
					
					// Get owner
					sprintf(buffer, "/proc/%s", dirp->d_name);
					struct stat proc;
					stat(buffer, &proc);
					
					if( !(uinfo = getpwuid(proc.st_uid)) ) {
						REP_ERROR(ERR_PSWD, ERRNO_MSG);
						destroy(entries);
						break;
					}

					sprintf(entry, "%s %s", entry, uinfo->pw_name);

					strcat(buffer, "/cmdline");
					FILE* fcmd = fopen(buffer, "r");
					if(fcmd) {
						fgets(buffer, BUFSIZ, fcmd);
						fclose(fcmd);
					}
					else {
						buffer[0] = '\0';
					}

					sprintf(entry, "%s %s", entry, buffer);
					if( !push(entries, entry) ) { destroy(entries); break; }
				}

				closedir(dir);

				qsort(entries->items, entries->nsize, sizeof(char*), &uid_cmpr);

				// Map and print
				char fstr[4];
				sprintf(fstr, "%%%ds", max_len);

				for(int i = 0; i < entries->nsize; i++) {
					char* entry = ((char**) entries->items)[i];
					
					// PID
					sscanf(entry, "%s", buffer);
					printf(fstr, buffer);
					
					puts(entry + strlen(buffer) + 1);
				} 
				destroy(entries);
				
				break;
			
			default:
				vector* args = make_vector(sizeof(char*));
				if(!args) { break; }
				
				int nset;
				char* cursor = (char*) buffer;

				do {
					char tmp = '\0';
					char* entry = (char*) malloc(sizeof(char) * ENTRY_MAX);
					if(!entry) { REP_ERROR(ERR_MLOC, ERRNO_MSG); destroy(args); break; }
					
					nset = sscanf(cursor, "%s%1s", entry, &tmp);

					if( !push(args, entry) ) { destroy(args); break; }
					cursor += strlen(entry);
					cursor = strchr(cursor, (int) tmp);
				} while(nset > 1);

				if( !push(args, NULL) ) { destroy(args); break; }
				
				pid_t fpid = fork();
				switch(fpid) {
					case -1:
						REP_ERROR(ERR_FORK, ERRNO_MSG);
						break;
					case 0:
						char** argv = (char**) args->items;
						execvp(argv[0], argv);

						REP_ERROR(ERR_EXEC, ERRNO_MSG);
						destroy(args);
						free(cwd);
						exit(EXIT_FAILURE);
					default:
						SIG_LOCK = 0;
						int status;
						
						if( sigaction(SIGINT, &ignore, &def) ) {
							REP_ERROR(ERR_SIGH, ERRNO_MSG);
						}
						
						if( waitpid(fpid, &status, 0) == -1 && !SIG_LOCK ) { 
							REP_ERROR(ERR_WAIT, ERRNO_MSG);
						}

						if( WEXITSTATUS(status) != EXIT_FAILURE && sigaction(SIGINT, &def, NULL) ) {
							REP_ERROR(ERR_SIGH, ERRNO_MSG);
						}		
				}
				destroy(args);
		}
	}

	return 0;
}
