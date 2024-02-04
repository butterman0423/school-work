
/*  Name: Nathaniel Escaro
    Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

.global count_specs

// count_specs(char *)
// Returns the number of "%a" in char *

// X19, X20: local vars

count_specs:
	SUB SP, SP, 16
	STR X19, [SP]
	STR X20, [SP, 8]

	MOV X19, X0		// Move str addr, to free register
	MOV X0, 0		// Set our counter
	MOV W9, 0x6125

parse_loop:
	LDRB W20, [X19] // Load a single char
	CBZ W20, ret	// If it's \0, stop the parse
	
	LDRH W20, [X19]	// Load 2 chars from str
	CMP W20, W9	// Do chars - "%a"
	B.NE parse_inc		// if the chars != "%a" dont increase count

	ADD X0, X0, 1			// Increment otherwise

parse_inc:
	ADD X19, X19, 1		// Move str ptr
	B parse_loop
ret:
	// Dealloc
	LDR X19, [SP]
	LDR X20, [SP, 8]
	ADD SP, SP, 16
	
	// X0 is already set

	RET

/*
    Declare .data here if you need.
*/
