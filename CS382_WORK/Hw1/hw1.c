#include <stdio.h>

/*
   Nathaniel Escaro

   I pledge my honor that I have abided by the Stevens Honor System.
	
   Sorting Algo for TASK 3: Insertion Sort (Reversed)

   I want to be considered for bonus points in TASK 3
*/

void copy_str(char* src, char* dst) {

COPY_LOOP:
	*dst = *src;
	
	if(*src == '\0') {
		return;
	}

	src++;
	dst++;

	goto COPY_LOOP;
}

int dot_prod(char* vec_a, char* vec_b, int length, int size_elem) {
	int dotted = 0;
	int a, b;

LOOP_BEGIN:

	if(length == 0) {
		goto END;
	}

	a = *( (int *) vec_a );
	b = *( (int *) vec_b );

	dotted += a * b;
	
	vec_a += size_elem;
	vec_b += size_elem;
	length--;

	goto LOOP_BEGIN;

END:
	return dotted;
}

void sort_nib(int* arr, int length) {
	const int INT_SIZE = sizeof(int);
	const int INT_BITS = INT_SIZE * 8;
	const int NIB_2_INT = INT_SIZE * 2;
	const int * MAX_ARR = arr + length;

	char nibs[ length * INT_SIZE * 2 ];
	
	char * nibSlot = nibs; // "iterator" for nibs[]
	
	int bitsParsed = 0; // how many bits were processed
	char nibBuffer; // buffer for calculating a nibble
	int currInt;
	char * sortNibSlot;

	// ---- FOR LAST STEP ONLY ---- //
	
	int intBuffer; // buffer for calculating an integer
	int nibParsed; // how many nibs were iterated through
	
	// --------------------------- //
	
INT_ITER:
	// Did we finish the last integer?
	if(arr == MAX_ARR) {
		arr -= length; // Reset arr to idx 0
		nibSlot = nibs; // Reset our nibs "iterator"
		
		goto PLACE;
	}

	currInt = *arr;
	arr++; // Move to next idx for the future
	
	nibBuffer = 0; // Prepare space for nibble
	bitsParsed = 0;

	goto NIB_BUILD;

NIB_BUILD:
	// Has a full integer been parsed already?
	if(bitsParsed != 0 && bitsParsed % INT_BITS == 0) {
		goto INT_ITER;
	}

	int bitPos = bitsParsed % 4;
	int bit = currInt & 1;

	nibBuffer = ( bit << bitPos ) | nibBuffer;
	
	currInt >>= 1;
	bitsParsed++;
	
	// Has 4 bits been parsed?
	if(bitsParsed % 4 == 0) {
		*nibSlot = nibBuffer; // Place our nibble
		sortNibSlot = nibSlot;
		goto INSERT; // Begin sorting
	}

	goto NIB_BUILD;

INSERT:
	// Is nibs sorted?
	if(sortNibSlot == nibs || *(sortNibSlot - 1) <= *sortNibSlot) {
		nibSlot++; // Move to next nibs idx
		nibBuffer = 0; // Reset our nibBuffer
		goto NIB_BUILD;
	}

	// Perform our swap
	char tmp = *sortNibSlot;
	*sortNibSlot = *(sortNibSlot - 1);
	*(sortNibSlot - 1) = tmp;

	sortNibSlot--; // Update our sorting pointer

	goto INSERT;

// ---------------------------- //

PLACE:
	// Are all integers set?
	if(arr == MAX_ARR) {
		return; // We're done
	}
	
	intBuffer = 0;
	nibParsed = 0;

	goto INT_BUILD;
	
INT_BUILD:
	// Have we got all the bytes need for an integer?
	if(nibParsed != 0 && nibParsed % NIB_2_INT == 0) {
		*arr = intBuffer; // Place our new integer
		arr++;
		goto PLACE;
	}

	intBuffer <<= 4; // Make space for nibble
	intBuffer |= *nibSlot; // Place our nib
	nibParsed++;
	
	nibSlot++; // Move to next nibble

	goto INT_BUILD;
}


int main(int argc, char** argv) {

    /**
     * Task 1
     */

    char str1[] = "382 is the best!";
    char str2[100] = {0};

    copy_str(str1,str2);
    puts(str1);
    puts(str2);

    /**
     * Task 2
     */

    int vec_a[3] = {12, 34, 10};
    int vec_b[3] = {10, 20, 30};
    int dot = dot_prod((char*)vec_a, (char*)vec_b, 3, sizeof(int));
    
    printf("%d\n",dot);

    /**
     * Task 3
     */

    int arr[3] = {0x12BFDA09, 0x9089CDBA, 0x56788910};
    sort_nib(arr, 3);
    for(int i = 0; i<3; i++) {
        printf("0x%x ", arr[i]);
    }
    puts(" ");

    return 0;
}
