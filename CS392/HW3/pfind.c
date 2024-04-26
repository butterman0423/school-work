/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PSTRING_ERRMSG "Error: Permissions string '%s' is invalid.\n"
#define MODE_MASK (S_IRWXU | S_IRWXG | S_IRWXO)
#define PSTRING_CHECK(expr) \
	if(expr) { \
		fprintf(stderr, PSTRING_ERRMSG, argv[2]); \
		exit(EXIT_FAILURE); \
	}
#define CHECK_NAME(name) \
	(name[0] != '.' || \
	name[1] != '\0' && (name[1] != '.' || name[2] != '\0'))

void traverse(struct stat*, struct dirent*, char*, int);

int main(int argc, char** argv) {
	char* pstr = argv[2];
	
	int pmode = 0;
	for(int i = 0; i < 9; i++) {
		// Check perm len is less
		PSTRING_CHECK(pstr[i] == '\0');
		
		pmode <<= 1;
		if(pstr[i] == '-') { continue; }
		
		// Check perm[i] letter
		switch(i % 3) {
			case 0:
				PSTRING_CHECK(pstr[i] != 'r');
				break;
			case 1:
				PSTRING_CHECK(pstr[i] != 'w');
				break;
			case 2:
				PSTRING_CHECK(pstr[i] != 'x');
		}
		
		pmode |= 2; // OR at 2nd LSB due to shift
	}
	
	// Check perm length is more
	PSTRING_CHECK( *(pstr + 9) != '\0' );
	pmode >>= 1; // Remove LSB zero

	struct stat finfo;
	struct dirent dinfo;
	traverse(&finfo, &dinfo, argv[1], pmode);

	return 0;
}

void traverse(struct stat* finfo, struct dirent* dinfo, char* path, int pmode) {
	chdir(path); // Enter dir
	DIR* dir_strm = opendir(".");
	char* stem = getcwd(NULL, 0);
	
	while( (dinfo = readdir(dir_strm)) ) {
		(void) stat( dinfo->d_name, finfo );

		mode_t mode = finfo->st_mode;

		// Enter if dir and not . or ..	
		if( S_ISDIR(mode) && CHECK_NAME(dinfo->d_name) ) {
			traverse(finfo, dinfo, dinfo->d_name, pmode);
			chdir(".."); // Return to this dir
			continue;
		}
		if( !S_ISREG(mode) || (mode & MODE_MASK) != pmode ) {
			continue;
		}
		
		// Print path
		printf("%s/%s\n", stem, dinfo->d_name);
	}

	closedir(dir_strm);
	free(stem);
}
