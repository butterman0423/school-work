/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <sys/wait.h>
#include <pthread.h>

#include "scull.h"

#define CDEV_NAME "/dev/scull"

/* Quantum command line option */
static int g_quantum;

static void* thread_route(void*);		// Code threads will run

static void usage(const char *cmd)
{
	printf("Usage: %s <command>\n"
	       "Commands:\n"
	       "  R          Reset quantum\n"
	       "  S <int>    Set quantum\n"
	       "  T <int>    Tell quantum\n"
	       "  G          Get quantum\n"
	       "  Q          Query quantum\n"
	       "  X <int>    Exchange quantum\n"
	       "  H <int>    Shift quantum\n"
		   "  i          Print process task_struct\n"
		   "  p          Fork 4 children to run command i twice\n"
		   "  t          Spawn 4 threads to run command i twice\n"
	       "  h          Print this message\n",
	       cmd);
}

typedef int cmd_t;

static cmd_t parse_arguments(int argc, const char **argv)
{
	cmd_t cmd;

	if (argc < 2) {
		fprintf(stderr, "%s: Invalid number of arguments\n", argv[0]);
		cmd = -1;
		goto ret;
	}

	/* Parse command and optional int argument */
	cmd = argv[1][0];
	switch (cmd) {
	case 'S':
	case 'T':
	case 'H':
	case 'X':
		if (argc < 3) {
			fprintf(stderr, "%s: Missing quantum\n", argv[0]);
			cmd = -1;
			break;
		}
		g_quantum = atoi(argv[2]);
		break;
	case 'R':
	case 'G':
	case 'Q':
	case 'h':

	case 'i':
	case 'p':
	case 't':

		break;
	default:
		fprintf(stderr, "%s: Invalid command\n", argv[0]);
		cmd = -1;
	}

ret:
	if (cmd < 0 || cmd == 'h') {
		usage(argv[0]);
		exit((cmd == 'h')? EXIT_SUCCESS : EXIT_FAILURE);
	}
	return cmd;
}

static int do_op(int fd, cmd_t cmd)
{
	int ret, q;
	struct task_info t_info;
	pid_t pids[4];
	pthread_t tids[4];

	switch (cmd) {
	case 'R':
		ret = ioctl(fd, SCULL_IOCRESET);
		if (ret == 0)
			printf("Quantum reset\n");
		break;
	case 'Q':
		q = ioctl(fd, SCULL_IOCQQUANTUM);
		printf("Quantum: %d\n", q);
		ret = 0;
		break;
	case 'G':
		ret = ioctl(fd, SCULL_IOCGQUANTUM, &q);
		if (ret == 0)
			printf("Quantum: %d\n", q);
		break;
	case 'T':
		ret = ioctl(fd, SCULL_IOCTQUANTUM, g_quantum);
		if (ret == 0)
			printf("Quantum set\n");
		break;
	case 'S':
		q = g_quantum;
		ret = ioctl(fd, SCULL_IOCSQUANTUM, &q);
		if (ret == 0)
			printf("Quantum set\n");
		break;
	case 'X':
		q = g_quantum;
		ret = ioctl(fd, SCULL_IOCXQUANTUM, &q);
		if (ret == 0)
			printf("Quantum exchanged, old quantum: %d\n", q);
		break;
	case 'H':
		q = ioctl(fd, SCULL_IOCHQUANTUM, g_quantum);
		printf("Quantum shifted, old quantum: %d\n", q);
		ret = 0;
		break;
	case 'i':
		ret = ioctl(fd, SCULL_IOCIQUANTUM, &t_info);
		if(ret == 0)
			printf( "state %u, cpu %u, prio %d, pid %d, tgid %d, nv %lu, niv %lu\n", \
					t_info.__state, \
					t_info.cpu, \
					t_info.prio, \
					t_info.pid, \
					t_info.tgid, \
					t_info.nvcsw, \
					t_info.nivcsw );
		break;
	case 'p':
		for(int i = 0; i < 4; i++) {
			pids[i] = fork();
			if(pids[i] == 0) {
				do_op(fd, (int) 'i');
				do_op(fd, (int) 'i');
				exit(0);
			}
		}
		
		// Wait for all children to die
		for(int i = 0; i < 4; i++) {
			waitpid( pids[i], NULL, 0 );
		}
		ret = 0;
		break;
	case 't':
		for(int i = 0; i < 4; i++) {
			pthread_create( (pthread_t *) tids + i, NULL, thread_route, &fd );
		}
		
		// Wait for all after created
		for(int i = 0; i < 4; i++) {
			pthread_join( tids[i], NULL );
		}

		ret = 0;
		break;
	default:
		/* Should never occur */
		abort();
		ret = -1; /* Keep the compiler happy */
	}

	if (ret != 0)
		perror("ioctl");
	return ret;
}

/* Body of function that threads will run */
static void* thread_route(void* arg) {
	int fd = *(int*) arg;
	do_op(fd, (int) 'i');
	do_op(fd, (int) 'i');
	pthread_exit(0);
}

int main(int argc, const char **argv)
{
	int fd, ret;
	cmd_t cmd;

	cmd = parse_arguments(argc, argv);

	fd = open(CDEV_NAME, O_RDONLY);
	if (fd < 0) {
		perror("cdev open");
		return EXIT_FAILURE;
	}

	printf("Device (%s) opened\n", CDEV_NAME);

	ret = do_op(fd, cmd);

	if (close(fd) != 0) {
		perror("cdev close");
		return EXIT_FAILURE;
	}

	printf("Device (%s) closed\n", CDEV_NAME);

	return (ret != 0)? EXIT_FAILURE : EXIT_SUCCESS;
}
