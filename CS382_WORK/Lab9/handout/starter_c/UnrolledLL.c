/**
 *
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
*/

#include "UnrolledLL.h"

uNode* new_unode(uNode** prev, u_int64_t blksize) {
	uNode* nnode = (uNode*) malloc(sizeof(uNode*));
	nnode->next = NULL;

	nnode->datagrp = (int *) malloc( sizeof(int) * blksize ); // Make int[blksize]
	nnode->blksize = blksize;
	
	// Parent if possible
	if(prev != NULL) {
		( *prev )->next = nnode;
	}

	return nnode;
}

void init_ullist(UnrolledLL* ullist, u_int64_t size, u_int64_t blksize) {
	ullist->head = NULL;
	ullist->len = 0;
	
	// Break if no nodes
	if(size == 0) {
		return;
	}
	
	uNode * curr = new_unode(NULL, blksize);
	size = size / blksize; // Actual number of nodes

	ullist->len = size;
	ullist->head = curr; // Save the head node
	
	while(1) {
		int * items = curr->datagrp;

		for(int i = 0; i < blksize; i++) {
			// Followed example on ArrayList and LL
			items[i] = rand() % 100;
		}
		
		size--;
		if(size == 0) {
			break;
		}

		curr = new_unode(&curr, blksize);
	}
}

void iterate_ullist(uNode* uiter) {
	while (uiter != NULL) {
		int * items = uiter->datagrp;
		u_int64_t len = uiter->blksize;

		for(int i = 0; i < len; i++) {
			// Followed example on ArrayList and LL
			int n = items[i];	
		}

		uiter = uiter->next;
	}
}

void clean_uulist(UnrolledLL* ullist) {
    if (ullist != NULL && ullist->head != NULL) {
        uNode* current = ullist->head;
        uNode* next;
 
        while (current != NULL) {
            next = current->next;
            free(current->datagrp);
            free(current);
            current = next;
        }

        ullist->head = NULL;
    }
    return;
}
