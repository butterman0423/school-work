/*

Nathaniel Escaro
I pledge my honor that I have abided by the Stevens Honor System.

*/

.text
.global _start
.extern printf

_start:
	ADR X0, side_a		// Load &side_a
	ADR X1, side_b		// Load &side_b
	ADR X2, side_c		// Load &side_c

	LDR X0, [X0]		// Load side_a value to X0
	LDR X1, [X1]		// Load side_b value to X1
	LDR X2, [X2]		// Load side_c value to X2

	MUL X0, X0, X0		// Calc a^2
	MUL X1, X1, X1		// Calc b^2
	MUL X2, X2, X2		// Calc c^2

	SUB X2, X2, X0		// Calc c^2 - a^2
	SUB X2, X2, X1		// Calc c^2 - a^2 - b^2

	CBZ X2, _get_yes	// Check if c^2=a^2+b^2 = 0, goto yes case if it is

	// Get no otherwise
	ADR X0, no			// Load &no
	B _print			// Move to print step

_get_yes:
	ADR X0, yes			// Load &yes

_print:
	BL printf			// Call printf() with string in X0
	
	MOV X0, 0			// Set exit status code 0
	MOV X8, 93			// Set system call exit()
	SVC 0				// Call exit()

.data
	side_a: .quad 3
	side_b: .quad 4
	side_c: .quad 5
	yes: .string "It is a right triangle.\n"
	len_yes: .quad . - yes // Calculate the length of string yes
	no: .string "It is not a right triangle.\n"
	len_no: .quad . - no // Calculate the length of string no
