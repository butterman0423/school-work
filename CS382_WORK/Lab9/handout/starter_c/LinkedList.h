#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct node {
	int data;
	struct node* next;
} Node;

typedef struct {
	Node* head;
	u_int64_t len;
} LinkedList;

Node* newnode(Node** prev);
void init_llist(LinkedList* llist, u_int64_t size);
void iterate_llist(Node* iter);
void clean_llist(LinkedList* llist);