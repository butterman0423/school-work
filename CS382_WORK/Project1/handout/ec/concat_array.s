
/*  Name: Nathaniel Escaro
    Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

.global concat_array
.extern itoascii
// concat_array(unsigned long int *, unsigned long int);
// Concatenates all contents of array together

// X19, X20, X21: local vars
// X30 will be saved

concat_array:
	SUB SP, SP, 32
	STR X30, [SP]
	STR X19, [SP, 8]
	STR X20, [SP, 16]
	STR X21, [SP, 24]

	MOV X19, X0		// Store long int *
	ADR X20, concat_array_outstr	// char *

arr_loop:
	CBZ X1, ret

	LDR X0, [X19] 	// Load arr[i]	
	BL itoascii		// itoascii(arr[i])


	// We can assume that itoascii() -> string of len >= 1
sset_loop:
	LDRB W21, [X0]
	STRB W21, [X20]
	ADD X0, X0, 1		// Move str ptr
	ADD X20, X20, 1		// Move buffer ptr	

	CBNZ W21, sset_loop // Break if \0 is reached

	// Replace \0 with ' '
	MOV W21, 32
	STRB W21, [X20, -1]

	//Move ptrs/len
	ADD X19, X19, 8
	SUB X1, X1, 1

	B arr_loop

ret:
	STRB WZR, [X20]	// Set \0 at end	

	// Dealloc
	LDR X30, [SP]
	LDR X19, [SP, 8]
	LDR X20, [SP, 16]
	LDR X21, [SP, 24]
	ADD SP, SP, 32

	ADR X0, concat_array_outstr		// Set ret value
	RET

.data
    /* Put the converted string into concat_array_outstrer,
       and return the address of concat_array_outstr */
    concat_array_outstr:  .fill 1024, 1, 0

