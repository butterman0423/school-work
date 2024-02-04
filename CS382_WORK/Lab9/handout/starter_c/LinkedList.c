#include "LinkedList.h"

Node* newnode(Node** prev) {
	Node* nnode = (Node*) malloc(sizeof(Node*));
	nnode->next = NULL;
	nnode->data = rand() % 100;
	if ((*prev) == NULL) *prev = nnode;
	else (*prev)->next = nnode;
	return nnode;
}

void init_llist(LinkedList* llist, u_int64_t size) {
	llist->head = NULL;
	llist->len = 0;
	Node* nnode;

	for (u_int64_t i = 0; i < size; i++) {
		if (i == 0) nnode = newnode(&(llist->head));
		else nnode = newnode(&nnode);
		llist->len++;
	}
}

void iterate_llist(Node* iter) {
	while (iter != NULL) {
		int num = iter->data;
		iter = iter->next;
	}
}


void clean_llist(LinkedList* llist) {
    if (llist != NULL && llist->head != NULL) {
        Node* current = llist->head;
        Node* next;
 
        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }

        llist->head = NULL;
    }
    return;
}