/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System
 */

.text
.global _start

_start:

/* Add your code here */
	ADR X0, numstr		// Get &numstr

	MOV X1, 0			// Index for numstr
	MOV X2, 0			// Buffer for the parsed num
	MOV X3, 10			// Store 10 for later multiplication	
_atoi:
	LDRB W10, [X0, X1]	// Load first char in numstr
	ADD X1, X1, 1		// Increment idx

	CBZ W10, _store_num	// If W10 is a \0, break out of loop

	SUB W10, W10, 48	// Do char W10 - '0'
	MUL X2, X2, X3		// Shift buffer by x10
	ADD X2, X2, X10		// Add the parsed digit
	B _atoi				// Continue the loop

_store_num:
	ADR X0, number		// Get &number
	STR X2, [X0]		// Store buffer into number

/* Do not change any part of the following code */
exit:
    MOV  X0, 1
    ADR  X1, number
    MOV  X2, 8
    MOV  X8, 64
    SVC  0
    MOV  X0, 0
    MOV  X8, 93
    SVC  0
    /* End of the code. */


/*
 * If you need additional data,
 * declare a .data segment and add the data here
 */






