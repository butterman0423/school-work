#include <unistd.h>
#include <stdlib.h>

typedef struct {
	u_int64_t len;
	int* dataarr;
} ArrayList;

void init_arr(ArrayList* array, u_int64_t size);
void iterate_arr(u_int64_t i, ArrayList* array);
void clean_arr(ArrayList* array);