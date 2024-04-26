/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
*/

.text
.global _start

_start:
	ADR X0, src_str		// Get &src_str
	ADR X1, dst_str		// Get &dst_str, store in X1 for printing later
	MOV X2, 0			// Our idx for src_str and dst_str

_char_loop:
	LDRB W3, [X0, X2]	// Load char at idx X2 from src_str

	STRB W3, [X1, X2]	// Copy char into dst_str at the same idx

	ADD X2, X2, 1		// Increment index
	CBNZ W3, _char_loop // Loop if char is not \0

	// Set Up For Printing
	MOV X0, 1			// Set status to printing
	ADD X2, X2, 1		// Add 1 to idx to get length
	
	MOV X8, 64			// System call for printing

	SVC 0				// Invoke the call
	
	// Ending
	MOV X0, 0			// Set status to 0
	MOV X8, 93			// System call for exit
	SVC 0				// Invoke the call


/*
 * If you need additional data,
 * declare a .data segment and add the data here
 */
