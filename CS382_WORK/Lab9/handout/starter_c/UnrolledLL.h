#include <unistd.h>
#include <stdlib.h>

typedef struct ull_node {
	int* datagrp;
	u_int64_t blksize;
	struct ull_node* next;
} uNode;

typedef struct {
	uNode* head;
	u_int64_t len;
} UnrolledLL;

uNode* new_unode(uNode** prev, u_int64_t blksize);
void init_ullist(UnrolledLL* ullist, u_int64_t size, u_int64_t blksize);
void iterate_ullist(uNode* uiter);
void clean_uulist(UnrolledLL* ullist);