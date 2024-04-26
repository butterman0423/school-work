#ifndef BCODE
#define BCODE

#include <stddef.h>
#include "pardef.h"

void setDataField(char **, int);
int translateLabel(Command *);

// Helpers
int getOpCode(Command *, char *);
int getMidByte(Command *, char *);
int getRm(Command *, char *);
int getRd(Command *, char *);
void putBits(char *, int, size_t);

#endif
