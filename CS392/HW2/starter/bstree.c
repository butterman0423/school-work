/*******************************************************************************
 * Name        : bstree.c
 * Author      : Nathaniel Escaro
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include "bstree.h"

/** Helper method that copies bytes from pFrom to pTo **/
static void set_val(void* pTo, void* pFrom, size_t bsize) {
	char* to = (char*) pTo;
	char* from = (char*) pFrom;
	for(size_t i = 0; i < bsize; i++) {
		to[i] = from[i];
	}
}

void add_node(void* pData, size_t bsize, tree_t* bst, int (*fcmpr)(void*, void*)) {
	if(!bst) { return; }
	
	node_t* curr = bst->root;

	// Empty tree, add to root
	if(!curr) {
		node_t* newNode = (node_t*) calloc(1, sizeof(node_t));	
		newNode->data = (void*) malloc(bsize);	
		set_val(newNode->data, pData, bsize);
		
		bst->root = newNode;
		return;
	}

	node_t* last;
	int diff;

	do {
		diff = fcmpr(pData, curr->data);
		last = curr;
		curr = diff < 0 ? curr->left : curr->right;
	} while(curr);

	if(diff < 0) {
		last->left = (node_t*) calloc(1, sizeof(node_t));
		last->left->data = (void*) malloc(bsize);
		set_val(last->left->data, pData, bsize);
		return;
	}
	last->right = (node_t*) calloc(1, sizeof(node_t));
	last->right->data = (void*) malloc(bsize);
	set_val(last->right->data, pData, bsize);
}

void print_tree(node_t* bst, void (*fprint)(void*)) {
	if(!bst) { return; }

	print_tree(bst->left, fprint);
	fprint(bst->data);
	print_tree(bst->right, fprint);
}

void destroy(tree_t* bst) {
	if( !bst || !(bst->root) ) { return; }
	
	node_t* tmp = bst->root;
	
	// Destroy left
	bst->root = tmp->left;
	destroy(bst);
	
	// Destroy right
	bst->root = tmp->right;
	destroy(bst);

	// Free the node
	free(tmp->data);
	free(tmp);
}
