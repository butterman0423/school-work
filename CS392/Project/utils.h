/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

#ifndef UTILS_HD
#define UTILS_HD


/** SHARED SIZE CONSTRAINTS **/

#define NAME_MAX		128
#define PROMPT_MAX		1024
#define OPT_MAX			50

// \n + Q + " ##: " + prompt + \n + 3 * ("#: " + OPT_MAX + "\n") + \0
#define SEND_MAX		28 + PROMPT_MAX + (3 * OPT_MAX)

#define SOCK_SIZE		sizeof(struct sockaddr_in)


/** EXCEPTIONS **/

#define PERR_MISS	"Missing argument for '-%c'.\n"
#define PERR_WHAT	"Unknown argument '-%c' recieved.\n"

#define REP_ERR(msg, ...) \
	fprintf(stderr, "Error: " msg __VA_OPT__(,) __VA_ARGS__)

#define CHK_FATAL(expr) \
	if(expr) { perror("Error"); exit(1); }

#define CHK_FATAL2(expr, msg, ...) \
	if(expr) { REP_ERR(msg, __VA_ARGS__); exit(1); }

/** COMMS **/

// Sockets
#define DEF_SOCK(stem) \
	int stem##_fd; \
	struct sockaddr_in stem##_addr

#define INIT_SOCK(stem, ip, port) \
	stem##_fd = socket(AF_INET, SOCK_STREAM, 0); \
	CHK_FATAL(stem##_fd == -1); \
	memset(&stem##_addr, 0, (size_t) SOCK_SIZE); \
	stem##_addr.sin_family = AF_INET; \
	stem##_addr.sin_port = htons(port); \
	stem##_addr.sin_addr.s_addr = inet_addr(ip)

#define SOCK_FD(stem)		stem##_fd
#define SOCK_ADDR(stem)		(struct sockaddr*) &stem##_addr

// select() args macros
#define SEL_RDON(rd)	&rd, NULL, NULL, NULL
#define SEL_WRON(wr)	NULL, &wr, NULL, NULL


/** Validation **/

int validate_port(void* dest, char* in);

#endif
