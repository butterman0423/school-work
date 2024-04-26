/*

Name: Nathaniel Escaro
Pledge: I pledge my honor that I have abided by the Stevens Honor System.

*/

.text
.global _start

_start:
	ADR  X0, vec1			// Base address of vec1
	ADR  X1, vec2			// Base address of vec2
	MOV  X5, 0				// Initialize our "dot" buffer

	LDR  X2, [X0]			// Load first coord of vec1
	LDR  X3, [X1]			// Load first coord of vec2

	MUL  X4, X2, X3			// Multiply the terms
	ADD  X5, X5, X4			// Add to the "dot" buffer
	
	LDR  X2, [X0, 8]		// Load second coord of vec1
	LDR  X3, [X1, 8]		// Load second coord of vec2

	MUL  X4, X2, X3			// Multiply the terms
	ADD  X5, X5, X4			// Add to the "dot" buffer

	LDR  X2, [X0, 16]		// Load third coord of vec1
	LDR  X3, [X1, 16]		// Load third coord of vec2

	MUL  X4, X2, X3			// Multiply the terms
	ADD  X5, X5, X4			// Add to the "dot" buffer

	ADR  X0, dot			// Fetch base address of dot
	STR  X5, [X0]			// Store buffer into global var

	// Exit the program with status 0
	MOV  X0, 0
	MOV  X8, 93
	SVC  0

.data
	vec1:	.quad	10, 20, 30
	vec2:	.quad	1, 2, 3
	dot:	.quad	0
