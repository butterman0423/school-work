/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System
 */


#include <unistd.h>
#include <stdio.h>
#define MY_SYSCALL 451

int main() {
	char large[] = "This is a string with possibly way more than 32 characters";
	char small[] = "Auolpteiy";

	printf("str1 before: %s\n", large);
	printf("syscall result: %ld\n", syscall(MY_SYSCALL, large));
	printf("str1 after: %s\n", large);
	
	printf("str2 before: %s\n", small);
	printf("syscall result: %ld\n", syscall(MY_SYSCALL, small));
	printf("str2 after: %s\n", small);

	return 0;
}
