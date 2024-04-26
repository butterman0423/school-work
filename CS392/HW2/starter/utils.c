/*******************************************************************************
 * Name        : utils.c
 * Author      : Nathaniel Escaro
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include "utils.h"

int cmpr_int(void* pA, void* pB) {
	int a = *((int*) pA); 
	int b = *((int*) pB);
	return (a > b) - (a < b);
}

int cmpr_float(void* pA, void* pB) {
	float a = *((float*) pA);
	float b = *((float*) pB);
	return (a > b) - (a < b);
}

void print_int(void* pN) {
	printf("%d ", *((int*) pN));
}

void print_float(void* pN) {
	printf("%f ", *((float*) pN));
}
