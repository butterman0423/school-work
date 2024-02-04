/*
* Nathaniel Escaro
* I pledge my honor that I have abided by the Stevens Honor System
*/

.text
.global _start

_start:
	MOV  X0, 1		// Set status to 1 (printing)
	ADR  X1, msg	// Store string address to X1
		
	ADR  X2, length // Store the length address to X2
	LDR  X2, [X2]	// Overwrite X2 with the value of length
	
	MOV  X8, 64		// Push system call #64 printing()

	SVC  0			// Invoke the call

	MOV  X0, 0		// Set status to 0
	MOV  X8, 93		// Push system call #93 exit()
	SVC  0			// Invoke the call

.data
	msg: .string "Hello World!\n"	// String to print out
	length: .quad 13	// Length of msg (in bytes).
