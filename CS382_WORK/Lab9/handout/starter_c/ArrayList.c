#include "ArrayList.h"

void init_arr(ArrayList* array, u_int64_t size) {
	array->len = 0;
	array->dataarr = (int*) malloc(sizeof(int) * size);
	for (u_int64_t i = 0; i < size; i++) {
		array->dataarr[i] = rand() % 100;
	}
}

void iterate_arr(u_int64_t i, ArrayList* array) {
	while (i < array->len) {
		int num = array->dataarr[i];
		i++;
	}
}

void clean_arr(ArrayList* array) {
    if (array != NULL) {
        free(array->dataarr);
    }
    return;
}