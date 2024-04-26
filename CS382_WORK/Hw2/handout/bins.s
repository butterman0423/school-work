/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
*/


.text
.global _start

_start:
	ADR X0, arr 		// Get &arr
	
	ADR X1, target		// Get &target
	LDR X1, [X1]		// Load target into X1
	
	ADR X3, length		// Get &length
	LDR X3, [X3]		// Load length into X5; Our upper bounds

	MOV X2, 0			// Our lower bounds
	MOV X4, 8			// Bytes in .quad

_bin_search:
	SUBS X10, X3, X2	// Do upper - lower, and set condition code

	B.LE _failed_case	// If upper - lower < 0, then the item does not exist

	LSR X10, X10, 1		// Divide by 2
	ADD X10, X2, X10	// Do lower + mid
	MUL X13, X10, X4	// Covert X10 idx to offset in bytes

	LDR X11, [X0, X13]	// Retrieve arr[X10]

	CMP X1, X11			// Do target - arr[X10]
	
	MOV X12, X3			// Temp container for upper bound
	
	MOV X3, X10			// Set the upper to the mid index
	B.LT _bin_search	// Continue search with this bound if target < arr[X10]

	MOV X3, X12			// Revert the change on upper
	MOV X2, X10			// Set lower bounds to mid index
	ADD X2, X2, 1		// Add 1 to lower
	B.GT _bin_search	// Continue search with this bound if target > arr[X10]

	// Otherwise target == arr[X10]
	ADR X1, msg1		// Get &msg1
	MOV X2, 24			// Length of msg1 + \0
	B _print			// Move to print step

_failed_case:
	ADR X1, msg2		// Get &msg2
	MOV X2, 28			// Length of msg2 + \0

_print:
	MOV X0, 1			// Set status for printing
	MOV X8, 64			// System call for printing
	SVC 0				// Invoke the call

	// Ending
	MOV X0, 0			// Set status to 0
	MOV X8, 93			// System call for exit
	SVC 0				// Invoke te call	

/*
 * If you need additional data,
 * declare a .data segment and add the data here
 */
