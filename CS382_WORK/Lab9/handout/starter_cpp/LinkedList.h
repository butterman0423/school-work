#include <ctime>
#include <cstdlib>

class Node {
public:
	int data;
	Node* next;

	Node(Node* prev);
};

class LinkedList {
public:
	Node* head;
	u_int64_t len;

	void init_llist(u_int64_t size);
	void iterate_llist();
};