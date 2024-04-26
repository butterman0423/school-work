/*
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 * */

#include <stdio.h>
#include <stdlib.h>

const int32_t MSB_MASK = 1 << 31;

void display(int8_t bit) {
	putchar(bit + 48);
}

void display_32(int32_t num) {
	int t = 32; // 32 bit integer
	
BIT_LOOP:
	// Get MSB and cast it to range [0, 1]
	int8_t bit = ~( (num & MSB_MASK) >> 31 ) + 1;
	display(bit);	
	
	num <<= 1;
	t--;

	if(t > 0) {
		goto BIT_LOOP;
	}
}

int main(int argc, char const *argv[]) {
	if( argc == 1 ) {
		display_32(382);
		printf("\n");
		return 0;
	}	

	// goto loop for cmd testing
	
	argv++;
TEST_LOOP:
	
	display_32( atoi(*argv) );
	printf("\n");

	argv++;
	argc--;
	if(argc > 1) {
		goto TEST_LOOP;
	}	
	
	return 0;
}
