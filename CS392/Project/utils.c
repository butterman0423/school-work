/**
 * Name: Nathaniel Escaro
 * Pledge: I pladge my honor that I have abided by the Stevens Honor System.
 */

#include <stdio.h>

#include "utils.h"

int validate_port(void* dest, char* in) {
	char tmp;
	int tmp_d = -1;
	int res = sscanf(in, "%d%c", &tmp_d, &tmp);
	sscanf(in, "%hu", (unsigned short*) dest);
	
	return res == 2 || tmp_d <= -1 || tmp_d > 65535;

}
