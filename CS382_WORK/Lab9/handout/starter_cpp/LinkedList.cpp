#include "LinkedList.h"

Node::Node(Node* prev) {
	next = nullptr;
	data = rand() % 100;
	if (prev == nullptr)
		prev = this;
	else
		prev->next = this;
}

void LinkedList::init_llist(u_int64_t size) {
	head = nullptr;
	len = 0;
	Node* nnode;

	for (u_int64_t i = 0; i < size; i++) {
		if (i == 0)
			nnode = new Node(head);
		else
			nnode = new Node(nnode);
		len++;
	}
}

void LinkedList::iterate_llist() {
	Node* iter = head;
	while (iter != nullptr) {
		int num = iter->data;
		iter = iter->next;
	}
}