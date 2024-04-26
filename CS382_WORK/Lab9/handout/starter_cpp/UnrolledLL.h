#include <unistd.h>
#include <cstdlib>

class uNode {
public:
	int* datagrp;
	u_int64_t blksize;
	uNode* next;

	uNode(uNode* prev, u_int64_t blksize);
};

class UnrolledLL {
public:
	uNode* head;
	u_int64_t len;

	void init_ullist(u_int64_t size, u_int64_t blksize);
	void iterate_ullist();
};