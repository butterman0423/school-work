/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"


#define ENTRIES_MAX		50
#define PLAYERS_MAX		3

#define USE_MSG			"Usage: %s [-f question_file] [-i IP_address] [-p port_number] [-h]\n" \
						"\n" \
						"  -f question file     Default to \"questions.txt\";\n" \
						"  -i IP_address        Default to \"127.0.0.1\";\n" \
						"  -p port_number       Default to 25555;\n" \
						"  -h                   Display this help info.\n"

#define plrs_for_each(i, set, plrs) \
	for(i = 0; i < PLAYERS_MAX; i++) \
		if( plrs[i].fd > -1 && FD_ISSET(plrs[i].fd, &set) )


struct Entry {
	char prompt[PROMPT_MAX];
	char options[3][OPT_MAX];
	int answer_idx;
};

struct Player {
	int fd;
	int score;
	char name[NAME_MAX];
};

void parse_args(int argc, char** argv, char* fname, char* ip, uint16_t* port) {
	char flag;

	while( (flag=getopt(argc, argv, ":f:i:p:h")) != -1 ) {
	
		// Rough check for missing args in middle	
		if( flag != ':' && *argv[optind - 2] == '-' && *optarg == '-' ) {
			flag = ':';
			optopt = argv[optind - 2][1];	
		}
		
		switch(flag) {
			
			case 'f':
				memset(fname, 0, 256);
				sprintf(fname, "%s", optarg);
				break;
			
			case 'i':
				memset(ip, 0, 36);
				sprintf(ip, "%s", optarg);
				break;
			
			case 'p':
				if( validate_port(port, optarg) ) {
					REP_ERR("Provided an invalid port number: %s\n", optarg);
					exit(EXIT_FAILURE);
				}
				break;
			
			case 'h':
				printf(USE_MSG, argv[0]);
				exit(EXIT_SUCCESS);

			case ':':
				REP_ERR(PERR_MISS USE_MSG, optopt, argv[0]);
				exit(EXIT_FAILURE);

			case '?':
				REP_ERR(PERR_WHAT, optopt);
				exit(EXIT_FAILURE);
		}
	}
}

void read_db(char* fname, struct Entry* arr, int* arr_len) {
	struct Entry* entry;
	char ans_buf[50];
	int i;

	FILE* db = fopen(fname, "r");
	CHK_FATAL(!db);

	int len = 0;

	do {
		entry = arr + len;
		len++;
		
		CHK_FATAL2( !fgets(entry->prompt, 1024, db), \
					"Problem reading question from %s\n", fname);
		CHK_FATAL2( !fscanf(db, "%s%s%s", entry->options[0], entry->options[1], entry->options[2]), \
					"Problem reading answer choices from %s\n", fname);
		CHK_FATAL2( !fscanf(db, "%s", ans_buf), \
					"Problem reading answer from %s\n", fname);
		
		i = 0;
		if( !strcmp(ans_buf, entry->options[i++]) || \
			!strcmp(ans_buf, entry->options[i++]) || \
			!strcmp(ans_buf, entry->options[i]) ) {
			
			entry->answer_idx = i;
			CHK_FATAL2( fseek(db, 1, SEEK_CUR), \
				   		"Failed to advance to next question\n" );	// Get rid of non-read \n
			continue;
		}
		
		REP_ERR("Answer choice does not match provided options: %s\n", ans_buf);
		exit(1);

	} while( len < ENTRIES_MAX && fgetc(db) == '\n' );
	
	CHK_FATAL2( !feof(db), \
				"%s has more than %d entries or is formatted incorrectly.\n", fname, ENTRIES_MAX );
	fclose(db);					// Could fail, but shouldn't matter

	*arr_len = len;
}

/** Modified function in textbook **/
int prep_fdset(fd_set* set, int* init, struct Player* plrs, int csize) {
	int maxfd = -1;

	FD_ZERO(set);
	
	if(init) {
		FD_SET(*init, set);
		maxfd = *init;
	}

	for(int i = 0; i < csize; i++) {
		if(plrs[i].fd > -1)
			FD_SET(plrs[i].fd, set);
		if(plrs[i].fd > maxfd)
			maxfd = plrs[i].fd;
	}

	return maxfd;
} 

void get_plrs(int server, struct Player* plrs) {
	fd_set fd_ready;
	int maxfd;
	int newfd;
	struct sockaddr_in inaddr;
	socklen_t addr_size = sizeof(inaddr);

	int i;
	int pending = 0;
	int ready = 0;
	char buf[NAME_MAX];
	int reg_map[PLAYERS_MAX] = {0};

	while(ready < PLAYERS_MAX) {
		maxfd = prep_fdset(&fd_ready, &server, plrs, PLAYERS_MAX);
		select( maxfd + 1, SEL_RDON(fd_ready) );

		if( FD_ISSET(server, &fd_ready) ) {
			newfd = accept( server, \
							(struct sockaddr*) &inaddr, \
							&addr_size );
			
			if(pending >= PLAYERS_MAX) {
				fprintf(stderr, "Max connection reached!\n");
				i = EOF;
				send(newfd, (char*) &i, 1, MSG_DONTWAIT);
				close(newfd);
			}
			else {
				pending++;
				for(i = 0; i < PLAYERS_MAX; i++) {
					if(plrs[i].fd == -1) {
						plrs[i].fd = newfd;
						printf("New connection detected!\n");
						send(newfd, "Please enter your name: ", 25, MSG_DONTWAIT);
						break;
					}
				}
			}	
		}
		
		plrs_for_each(i, fd_ready, plrs) {
			memset(buf, 0, NAME_MAX);
			switch( read(plrs[i].fd, buf, NAME_MAX) ) {
				
				case -1:
					perror("Error");
				
				case 0:
					close(plrs[i].fd);
					plrs[i].fd = -1;
					pending--;
					
					// Player was registered, but disconnected.
					for(int j = 0; j < ready; j++) {
						if( !strcmp(plrs[j].name, plrs[i].name) ) {
							fprintf(stderr, "Lost connection with '%s'!\n", plrs[j].name);
							plrs[j].name[0] = '\0';
							ready--;
							reg_map[i] = 0;
							break;
						}
					}
					break;

				default:
					if(reg_map[i]) continue; // User already registered

					plrs[ready].score = 0;

					memset(plrs[ready].name, 0, NAME_MAX);
					strcpy(plrs[ready].name, buf);
					printf("Hi %s!\n", plrs[ready].name);
					
					ready++;
					reg_map[i] = 1;
			}
		}
	}
}

// Originally, game would account of disconnected player
// Now, terminate game on disconnection
int play(struct Player* plrs, struct Entry* qs, int qsize, int* max_ptns) {
	fd_set fd_ready;
	int maxfd;

	struct Entry q;
	int i;
	
	size_t qlen;
	int tmp;

	struct Player* first_p;
	int first_ans;

	int curr_q = 0;

	while(curr_q < qsize) {
		char buf[SEND_MAX] = {0};

		maxfd = prep_fdset(&fd_ready, NULL, plrs, PLAYERS_MAX);
		
		/*
		// All players left...
		if(maxfd == -1) 
			return 0;
		*/

		q = qs[curr_q++];
		
		// Build entry
		sprintf( buf, "\nQuestion %d: %s\n1: %s\n2: %s\n3: %s\n", \
				 curr_q, q.prompt, \
				 q.options[0], q.options[1], q.options[2] );
		
		qlen = strlen(buf);
		fwrite(buf, sizeof(char), qlen, stdout);
		fflush(stdout);

		select(maxfd + 1, SEL_WRON(fd_ready));

		// Send question
		plrs_for_each(i, fd_ready, plrs) {
			if( write(plrs[i].fd, buf, qlen) <= 0 ) {
				perror("Error");
				REP_ERR("Failed to send question to %s.\n", plrs[i].name);
			}
		}

		// Retrieve first answer (and disconnections)
		first_p = NULL;
		while(maxfd > -1) {
			maxfd = prep_fdset(&fd_ready, NULL, plrs, PLAYERS_MAX);
			if(maxfd == -1) break;

			select(maxfd + 1, SEL_RDON(fd_ready));
			
			plrs_for_each(i, fd_ready, plrs) {
				tmp = -1;
				switch( read(plrs[i].fd, &tmp, sizeof(int)) ) {
					
					case -1:
						perror("Error");

					case 0:
						/*
						fprintf(stderr, "Lost connection with '%s'!\n", plrs[i].name);
						close(plrs[i].fd);
						plrs[i].fd = -1;
						
						continue;
						*/
						fprintf(stderr, "Lost connection!\n");
						return 0;

					default:
						if(!first_p && tmp > -1) {
							plrs[i].score += tmp == q.answer_idx ? 1 : -1;
							maxfd = -1;
							first_p = plrs + i;
							first_ans = tmp;
						}
				}
			}
		}

		/*
		// We still have players right?
		if(first_p == NULL)
			return 0;
		*/
		memset(buf, 0, SEND_MAX);
		
		// Build sendback msg
		sprintf(buf, "\n'%s' was the first to respond with... %s!\n", \
				first_p->name, q.options[first_ans]);
		sprintf((char*) buf + strlen(buf), "And the correct answer was %s!\n", \
				q.options[q.answer_idx]);

		qlen = strlen(buf);

		fwrite(buf, sizeof(char), qlen, stdout);
		fflush(stdout);

		maxfd = prep_fdset(&fd_ready, NULL, plrs, PLAYERS_MAX);
		select(maxfd + 1, SEL_WRON(fd_ready));
		
		// Notify user of correct answer (and answeree)
		plrs_for_each(i, fd_ready, plrs) {
			if( write(plrs[i].fd, buf, qlen) == -1 ) {
				perror("Error");
				REP_ERR("Could not report answer to %s\n", plrs[i].name);
			}
		}
	}

	for(i = 0; i < PLAYERS_MAX; i++) {
		if(plrs[i].fd > -1 && plrs[i].score > *max_ptns) {
			*max_ptns = plrs[i].score;
		}
	}

	return 1;
}

int main(int argc, char** argv) {
	DEF_SOCK(me);

	struct Entry entries[ENTRIES_MAX];
	int entries_len;
	
	int max_ptns;
	struct Player plrs[PLAYERS_MAX];

	char c_fname[256] = "questions.txt";
	char c_ip[36] = "127.0.0.1";		// Only care about IPv4
	uint16_t c_port = 25555;
	
	
	/** Data Loading **/
	parse_args(argc, argv, c_fname, c_ip,  &c_port);
	read_db(c_fname, entries, &entries_len);

	
	/** Server Creation **/
	INIT_SOCK(me, c_ip, c_port);
	
	CHK_FATAL( bind( SOCK_FD(me), SOCK_ADDR(me), SOCK_SIZE) == -1 );
	CHK_FATAL( listen( SOCK_FD(me), PLAYERS_MAX) == -1 );
	
	printf("Welcome to 392 Trivia!\n");


	/** Accepting Users **/
	for(int i = 0; i < PLAYERS_MAX; i++) {
		plrs[i].fd = -1;
		plrs[i].name[0] = '\0';
	}
	get_plrs(me_fd, plrs);


	/** Game Start **/
	printf("The game starts now!\n");

	max_ptns = -entries_len - 1;
	if( play(plrs, entries, entries_len, &max_ptns) ) {
		char buf[SEND_MAX] = "Congrats, ";

		for(int i = 0; i < PLAYERS_MAX; i++) {
			if(plrs[i].fd > -1 && plrs[i].score == max_ptns)
				sprintf(buf + strlen(buf), "'%s' ", plrs[i].name);
		}

		sprintf(buf + strlen(buf), "for winning with the highest score of %d!\n", max_ptns);
		
		fwrite(buf, sizeof(char), SEND_MAX, stdout);
		for(int i = 0; i < PLAYERS_MAX; i++) {
			if(plrs[i].fd > -1)
				send(plrs[i].fd, buf, SEND_MAX, MSG_DONTWAIT);
		}
	}
	else {
		//printf("All players left, no winner\n");
		printf("A player disconnected... Ending Game\n");
	}


	/** Cleanup **/
	for(int i = 0; i < PLAYERS_MAX; i++) {
		if(plrs[i].fd > -1)
			close(plrs[i].fd);
	}

	close(me_fd);

	return 0;
}
