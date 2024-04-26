/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"


#define USE_MSG		"Usage: %s [-i IP_address] [-p port_number] [-h]\n" \
					"\n" \
					"  -i IP_address        Default to \"127.0.0.1\";\n" \
					"  -p port_number       Default to 25555;\n" \
					"  -h                   Display this help info.\n"


void parse_args(int argc, char** argv, char* ip, uint16_t* port) {
	char flag;

	while( (flag=getopt(argc, argv, ":i:p:h")) != -1 ) {
		
		// Rough check for missing args in middle	
		if( flag != ':' && *argv[optind - 2] == '-' && *optarg == '-' ) {
			flag = ':';
			optopt = argv[optind - 2][1];	
		}
		
		switch(flag) {
	

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
				REP_ERR(PERR_MISS USE_MSG, (int) *optarg, argv[0]);
				exit(EXIT_FAILURE);

			case '?':
				REP_ERR(PERR_WHAT, (int) *optarg);
				exit(EXIT_FAILURE);
		}
	}
}

int prep_fdset(fd_set* set, int svr_fd) {
	FD_ZERO(set);
	FD_SET(svr_fd, set);
	FD_SET(0, set);

	return svr_fd;
};

void play(int svr_fd) {
	fd_set fd_ready;
	int maxfd;
	int ans;

	char buf[SEND_MAX];
 	int end = 0;
	while(!end) {
		memset(buf, 0, SEND_MAX);
		maxfd = prep_fdset(&fd_ready, svr_fd);
		select(maxfd + 1, SEL_RDON(fd_ready));
		
		if( FD_ISSET(svr_fd, &fd_ready) ) {
			switch( read(svr_fd, buf, SEND_MAX) ) {
				
				case -1:
				case 0: // Game is over :)
					end = 1;
					continue;

				default:
					printf("%s", buf);
					fflush(stdout);
			}
		}
		else {
			buf[0] = '\0';

			switch( read(0, buf, 1) ) {
				
				case -1:
					perror("Error: ");
					REP_ERR("Failed to read input.\n");
					break;
				
				case 0:
				default:
					ans = buf[0] - '0';

					if(ans < 1 || ans > 3) {
						REP_ERR("Input must be either (1), (2), or (3)\n");
						continue;
					}
					--ans;
					send(svr_fd, &ans, sizeof(int), MSG_DONTWAIT);
					
					// Clear stdin buffer
					while( (ans=read(0, buf, 1)) != -1 && buf[0] != '\n' );
					if(ans == -1) perror("Error: Failed to clear stdin");
			}
		}
	}
};

int main(int argc, char** argv) {
	DEF_SOCK(svr);
	
	char name_buf[NAME_MAX] = {0};

	char c_ip[36] = "127.0.0.1";
	uint16_t c_port = 25555;


	/** Config Loading **/
	parse_args(argc, argv, c_ip, &c_port);

	
	/** Connection **/
	INIT_SOCK(svr, c_ip, c_port);
	CHK_FATAL( connect( SOCK_FD(svr), SOCK_ADDR(svr), SOCK_SIZE) == -1 );


	if( read( SOCK_FD(svr), name_buf, 25) == -1 || name_buf[0] == EOF ) {
		REP_ERR("Server refused your connection.\n");
		close( SOCK_FD(svr) );
		exit(1);
	}


	fwrite(name_buf, sizeof(char), 25, stdout);
	fflush(stdout);
	
	scanf("%s", name_buf);

	if( name_buf[0] == '\n' ) {
		REP_ERR("No name given\n");
		close( SOCK_FD(svr) );
		exit(1);
	}

	send( SOCK_FD(svr), name_buf, strlen(name_buf), 0 );
	
	/** Game **/
	play( SOCK_FD(svr) );


	/** Cleanup **/
	close( SOCK_FD(svr) );


	return 0;
}
