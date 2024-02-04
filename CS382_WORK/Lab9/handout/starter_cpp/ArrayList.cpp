#include "ArrayList.h"

void ArrayList::init_arr(u_int64_t size) {
	len = 0;
	dataarr = new int[size];
	for (u_int64_t i = 0; i < size; i++) {
		dataarr[i] = rand() % 100;
	}
}

void ArrayList::iterate_arr(u_int64_t i) {
	while (i < len) {
		int num = dataarr[i];
		i++;
	}
}