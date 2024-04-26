
/*  
    Name: Nathaniel Escaro
    Pledge: I pledge my honor that I have abided by the Stevens Honor System
 */

.global itoascii


// itoascii(unsigned long int) -> .string
// converts all digits in X0 into a string
// returns the new string

// X19, X20, X21, X22: local vars
// These will be restored after call

itoascii:
	SUB SP, SP, 32	// Alloc space for local vars
	STR X19, [SP]
	STR X20, [SP, 8]
	STR X21, [SP, 16]
	STR X22, [SP, 24]

	ADR X19, buffer // Stores &buffer, treated as ptr
	MOV X20, 10		// Num 10

	// Run a do while loop
digi_loop:
	// Grab LSD
	UDIV X21, X0, X20	// XYZ / 10 = XY
	MUL X22, X21, X20	// XY * 10 = XY0
	SUB X22, X0, X22	// XYZ - XY0 = Z: the LSD
	
	ADD X22, X22, 48	// Convert int -> ascii
	STRB W22, [X19]		// Store to string
	ADD X19, X19, 1		// Move ptr
	MOV X0, X21			// Move to next digit

	// Is num 0? repeat if not
	CBNZ X0, digi_loop

	MOV X22, 0
	STRB W22, [X19]		// Store \0
	
	// Since num is parsed from LSD -> MSD, we need to reverse it
	SUB X19, X19, 1		// last digit
	ADR X0, buffer		// first digit

rev_digi:
	CMP X0, X19
	B.GT ret	// Break if left ptr > right ptr
	
	// Swap them
	LDRB W20, [X0]
	LDRB W21, [X19]
	
	STRB W21, [X0]
	STRB W20, [X19]

	// Move ptrs
	ADD X0, X0, 1
	SUB X19, X19, 1
	B rev_digi

ret:
	// Dealloc vars
	LDR X19, [SP]
	LDR X20, [SP, 8]
	LDR X21, [SP, 16]
	LDR X22, [SP, 24]
	ADD SP, SP, 32

	ADR X0, buffer 		// Set return value
	RET


.data
    /* Put the converted string into buffer,
       and return the address of buffer */
    buffer: .fill 128, 1, 0


