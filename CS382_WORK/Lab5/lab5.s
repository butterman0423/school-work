/**
* Name: Nathaniel Escaro
* Pledge: I pledge my honor that I have abided by the Stevens Honor System.
*/

.text
.global _start
.extern printf


/* char _uppercase(char lower) */
_uppercase:
	// Parameter: W0 = lower [char]
	// Assumption lower is in [a, z]

	SUB W0, W0, 32		// Cast it to uppercase
	RET

/* int _toupper(char* string) */
_toupper:
	SUB SP, SP, 24 		// Allocate 24 bytes
	STR X30, [SP]		// Store X30 at lowest space
	STR X2, [SP, 8]	
	STR X1, [SP, 16]

	//Prep variables
	MOV X2, 0			// Will store length
	MOV X1, X0			// Will store char *
	MOV X0, 0			// Will store char

upper_loop:
	LDRB W0, [X1, X2]	// Load curr char to W0
	CBZ W0, return		// Has the end been reached?

	BL _uppercase		// Call _uppercase(W0)
	
	STRB W0, [X1, X2]	// Store changed char to string
	ADD X2, X2, 1		// Add length
		
	B upper_loop

return:
	MOV X0, X2			// Set the return value
	
	// Restore old values
	LDR X30, [SP]
	LDR X2, [SP, 8]
	LDR X1, [SP, 16]

	ADD SP, SP, 24		// Dealloc stack
	
	RET

_start:
	ADR X0, str
	BL _toupper
	
	MOV X1, X0		// Move length to 2nd param
	ADR X0, outstr
	ADR X2, str		// Set the string to print
	BL printf  

    MOV  X0, 0
    MOV  X8, 93
    SVC  0


.data
str:    .string   "helloworld"
outstr: .string   "Converted %ld characters: %s\n"
