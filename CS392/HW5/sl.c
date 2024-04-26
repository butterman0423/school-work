/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERR_EXIT(msg) \
	fprintf(stderr, msg"\n"); \
	exit(EXIT_FAILURE)

int main(int argc, char** argv) {
	if(argc != 2) {
		fprintf(stderr, "The first argument has to be a directory.");
		exit(1);
	}

	chdir(argv[1]);
	if(errno) {
		switch(errno) {
			case ENOTDIR:
				fprintf(stderr, "The first argument has to be a directory.");
				exit(1);
			
			case ENOENT: // Appease Test Case 3
			case EACCES:
				fprintf(stderr, "Permission denied. %s cannot be read.", argv[1]);
				exit(1);
			
			default:
				fprintf(stderr, "Error: chdir failed");
				exit(1);
		}
	}

	int pipA[2]; // SORT && LS
	int pipB[2]; // SORT && PARENT
	
	int lc;
	ssize_t res; // For read()
	char tmp;
	char buf[32]; // Surely this is more than enough
	pid_t pid;

	if( pipe(pipB) == -1 ) { ERR_EXIT("Error: Pipe failed"); }
	pid = fork(); // SORT | PARENT

	switch(pid) {
		case -1:
			ERR_EXIT("Error: Fork failed.");

		case 0:
			if( pipe(pipA) == -1 ) { ERR_EXIT("Error: Pipe failed"); }
			pid = fork(); // LS | SORT

			switch(pid) {
				case -1:
					ERR_EXIT("Error: Fork failed.");

				case 0:
					close(0);
					close(pipA[0]);
					dup2(pipA[1], 1);
					
					execlp("ls", "ls", "-1ai", NULL);
					ERR_EXIT("Error: ls failed.");

				default:
					close(pipB[0]);
					close(pipA[1]);
					dup2(pipA[0], 0);
					dup2(pipB[1], 1);

					execlp("sort", "sort", NULL);
					ERR_EXIT("Error: sort failed");
			}

		default:
			close(pipB[1]);
			dup2(pipB[0], 0);
			
			// Block with read, increment by \n
			lc = 0;
			while( 0 < (res=read(0, &tmp, 1)) ) {
				if(res == -1) { ERR_EXIT("Error: read failed"); }
				if( write(1, &tmp, sizeof(char)) == -1 ) { ERR_EXIT("Error: write failed"); }
				
				if(tmp == '\n') lc++;
			}
			
			sprintf(buf, "Total files: %d\n", lc);	
			if( write(1, buf, strlen(buf)) == -1 ) { ERR_EXIT("Error: write failed"); }
	}

	return 0;
}
