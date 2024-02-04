
/*  Name: Nathaniel Escaro
    Pledge: I pledge my honor that I have abided by the Stevens Honor System

	I would like to be graded for EC.
 */

.global pringle
.extern count_specs
.extern concat_array

// HELPERS //

// _fill_vargs(unsigned long int *, ...) -> void
// Fills the stack allocated array with values in ...
// X0 is base addr of array
// First element in array must be length of array (in bytes)
// Assumption: length >= 48

_fill_vargs:
	LDR X9, [X0]	// Get length of array
	
	// Store first 6 args
	STR X1, [X0]
	STR X2, [X0, 8]
	STR X3, [X0, 16]
	STR X4, [X0, 24]
	STR X5, [X0, 32]
	STR X6, [X0, 40]
	
	// Move to stack args (if any)	
	ADD X10, X9, 48		// start of stack args
	ADD X10, X0, X10	// move to ptr

	SUB X9, X9, 48
	ADD X0, X0, 48	// Move array ptr
	B.GT stack_fill

	RET

stack_fill:
	STR X7, [X0]
	LDR X11, [X10]
	STR X11, [X0, 8]
	
	SUBS X9, X9, 16
	ADD X10, X10, 8
	ADD X0, X0, 16

	B.NE stack_fill_loop
	RET
	
stack_fill_loop:
	LDR X11, [X10]		// arr
	LDR X12, [X10, 8]	// arr len
	
	STR X11, [X0]
	STR X12, [X0, 8]

	SUBS X9, X9, 16
	ADD X10, X10, 16
	ADD X0, X0, 16
	B.NE stack_fill_loop		// X9 != 0

	RET

// ------- //

// _print_str(char *) -> void
// Prints all characters in a string
// X8 is assumed and must be 64

_print_str:
	MOV X1, X0
	MOV X0, 1

	MOV X2, 0
	MOV X10, X1
	MOV X11, 0

find_len:
	LDRB W9, [X10]
	CBZ W9, print

	ADD X2, X2, 1
	ADD X10, X10, 1
	ADD X11, X11, 1
	B find_len

print:
	SVC 0
	MOV X0, X11		// Return chars printed
	RET


// pringle(char *, ...)
// Prints a formatted string

// X1 - X7 are params, any overflow is in stack

pringle:
	// Allocate initial stack
	SUB SP, SP, 48
	STR X30, [SP]
	STR X19, [SP, 8]
	STR X20, [SP, 16]
	STR X21, [SP, 24]
	STR X22, [SP, 32]
	STR X23, [SP, 40]

	MOV X19, X0		// Move str to X19
	BL count_specs	// Get num of specifiers

	CMP X0, 3
	B.GE arg_fill
	MOV X0, 3		// Clamp num args to 3 for stack fill

arg_fill:
	LSL X0, X0, 4	// count * 2 * 8 = free bytes in alloc array
	MOV X20, X0		// Save num of bytes

	SUB SP, SP, X0	// Store args in a stack-allocated array
	STR X0, [SP]	// Store length in first element
	MOV X0, SP

	BL _fill_vargs	// Fill it

	MOV X21, SP	// ptr to vargs array
	MOV X9, 0	// Stores two adjacent chars
	
	// Set up write config
	MOV X0, 1
	MOV X8, 64
	ADR X1, char_buffer
	MOV X2, 1

	MOV X22, 0x6125		// = %a
	MOV X23, 0			// Num chars printed

	// Begin parse
parse_loop:
	LDRB W9, [X19]
	CBZ W9, ret

	LDRH W9, [X19]
	CMP W9, W22
	B.EQ print_arr

	STRB W9, [X1]
	SVC 0
	ADD X23, X23, 1		// +1 char printed

	ADD X19, X19, 1		// Increment str ptr
	B parse_loop

print_arr:
	LDR X0, [X21]		// Get array
	LDR X1, [X21, 8]	// Get array length
	BL concat_array		// Stringify array
	BL _print_str

	ADD X23, X23, X0	// +X0 chars printed
	
	// Reset write settings
	MOV X0, 1
	ADR X1, char_buffer
	MOV X2, 1
	
	ADD X19, X19, 2		// Increment str ptr
	ADD X21, X21, 16	// Increment vargs ptr
	B parse_loop

ret:
	// Print the null character
	STRB W9, [X1]
	SVC 0
	
	MOV X0, X23			// Set return val

	// Dealloc vargs
	ADD SP, SP, X20
	
	// Dealloc stack vars
	LDR X30, [SP]
	LDR X19, [SP, 8]
	LDR X20, [SP, 16]
	LDR X21, [SP, 24]
	LDR X22, [SP, 32]
	LDR X23, [SP, 40]
	ADD SP, SP, 48

	RET	// Yay

/*
    Declare .data here if you need.
*/
.data
char_buffer: .ascii "\0"	// Stores 1 character

