#ifndef PARSER
#define PARSER

#include "pardef.h"

int parseDest(const char *, Command *);
int parseFunc(const char *, Command *);
int parseArgs(const char *, const char *, Command *);
int parseByte(const char *, int *);

#endif
