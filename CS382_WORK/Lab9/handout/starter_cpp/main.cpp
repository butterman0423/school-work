#include <iostream>
#include <ctime>
#include <unistd.h>
#include <getopt.h>
#include <cctype>
#include <cstring>

#include "LinkedList.h"
#include "ArrayList.h"
#include "UnrolledLL.h"

void usage(const char* first) {
	printf("Usage: %s [options]\n", first);
	printf("Options:\n");
	printf("  -t {ll|arr|ull}: run test on structures;\n");
	printf("  -s <size>: the number of elements stored in each structure; default to 100;\n");
	printf("  -b <blksize>: the number of elements stored in unrolled linked list node; default to 1;\n");
	printf("  -h: show this usage manual.\n");
}

void parser(int argc, char** argv,
			char tasks[][5], u_int64_t* _blksize, u_int64_t* _size) {

	char c;
	char* task = nullptr;
	u_int64_t blksize = 1;
	u_int64_t size = 1000;

	if (argc < 2) {
		usage(argv[0]);
		exit(1);
	}

	while ((c = getopt(argc, argv, "t:s:b:h")) != (char) -1) {
		switch (c) {
			case 't':
				task = optarg;
				break;
			case 'h':
				usage(argv[0]);
				exit(0);
			case 'b':
				blksize = atoi(optarg);
				break;
			case 's':
				size = atoi(optarg);
				break;
			case '?':
				if (optopt == 't' || optopt == 's' || optopt == 'b')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				usage(argv[0]);
				exit(1);
			default:
				usage(argv[0]);
				exit(1);
		}
	}

	if (blksize > size) {
		fprintf(stderr, "Block size cannot be larger than size.\n");
		exit(1);
	}

	if (blksize <= 0 || size <= 0) {
		fprintf(stderr, "Block size and size have to be positive.\n");
		exit(1);
	}

	*_blksize = blksize;
	*_size = size;

	char* pch;
	int i = 0;
	pch = strtok(task, ",");
	while (pch != nullptr) {
		if (strlen(pch) < 2) break;
		if (strcmp(pch, "ll") && strcmp(pch, "ull") && strcmp(pch, "arr")) {
			fprintf(stderr, "Option %s for -t is invalid.\n", pch);
			usage(argv[0]);
			exit(1);
		}
		strcpy(tasks[i], pch);
		pch = strtok(nullptr, ",");
		i++;
	}
}


void test_linkedlist(u_int64_t size) {
	clock_t start;
	double execution_time;
	LinkedList llist;
	llist.init_llist(size);
	start = clock();
	llist.iterate_llist();
	execution_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
	printf("Linked List:\n");
	printf("Length: %llu, Exec time: %lfs\n\n", size, execution_time);
}

void test_arraylist(u_int64_t size) {
	clock_t start;
	double execution_time;
	ArrayList arrList;
	arrList.init_arr(size);
	start = clock();
	arrList.iterate_arr(0);
	execution_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
	printf("Array List:\n");
	printf("Length: %llu, Exec time: %lfs\n\n", size, execution_time);
}

void test_unrolledllist(u_int64_t size, u_int64_t blksize) {
	clock_t start;
	double execution_time;
	UnrolledLL ullist;
	ullist.init_ullist(size, blksize);
	start = clock();
	ullist.iterate_ullist();
	execution_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
	printf("Unrolled Linked List:\n");
	printf("Length: %llu, Exec time: %lfs\n\n", size, execution_time);
}


int main(int argc, char** argv) {
	char tasks[3][5] = {{0}};
	u_int64_t blksize;
	u_int64_t size;

	parser(argc, argv, tasks, &blksize, &size);

	for (size_t i = 0; i < 3; i++) {
		if (strlen(tasks[i]) >= 2) {
			if (!strcasecmp(tasks[i], "ll")) {
				test_linkedlist(size);
			} else if (!strcasecmp(tasks[i], "arr")) {
				test_arraylist(size);
			} else if (!strcasecmp(tasks[i], "ull")) {
				test_unrolledllist(size, blksize);
			} else {
				fprintf(stderr, "Option %s for -t is invalid.\n", tasks[i]);
				usage(argv[0]);
				exit(1);
			}
		}
	}
	return 0;
}
