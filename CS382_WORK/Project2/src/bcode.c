#include <string.h>
#include "bcode.h"

typedef struct {
	char label[10];
	int addr;
} AddrPair;

// Only 32 labels will be allowed
AddrPair datamem[32];
int nextFree = 0;
int pairSize = 0;

int putLabel(char *label, int size) {
	if(pairSize >= 32) {
		return 1;
	}
	
	datamem[pairSize] = (AddrPair) {};
	strcpy( datamem[pairSize].label, label );
	datamem[pairSize].addr = nextFree;

	nextFree += size;
	pairSize++;
	
	return 0;
}

int translateLabel(Command *cmd) {
	// No label to translate
	if( *(cmd->dlabel) == '\0' ) {
		return 0;
	}

	// Linear search for hit
	for(int i = 0; i < pairSize; i++) {
		if( strcmp(datamem[i].label, cmd->dlabel) == 0 ) {
			cmd->bits = datamem[i].addr;
			return 0;
		}
	}

	return 1;
}

int getOpCode(Command *cmd, char *bits) {
	switch(cmd->f) {
		/*
		case SET_N:
			memcpy(bits, "10001000", 8);
			return 0;
		case SET_R:
			memcpy(bits, "10000000", 8);
			return 0;
		*/
		case SET:
			memcpy(bits, "10000000", 8);
			break;
		case STORE:
			memcpy(bits, "11011000", 8);
			return 0;
		case LOAD:
			memcpy(bits, "10111000", 8);
			return 0;
		case FLASH:
			memcpy(bits, "01000000", 8);
			return 0;
		case FLUSH:
			memcpy(bits, "00100000", 8);
			return 0;
		case DELAY:
			memcpy(bits, "00000000", 8);
			return 0;
		default:
			memcpy(bits, "10000000", 8);
			
			// Arithmetic Code
			int acode = (int) cmd->f;
			putBits(bits + 5, acode, 3);
	}
	
	// Rn or bit8?
	int hasRn = cmd->nreg != -1;
	int hasBits = cmd->bits != -1;

	if(hasRn == hasBits) {
		return 1; // This should not be possible
	}
	
	memset(bits + 4, hasBits + '0', 1);

	return 0;
}

int getMidByte(Command *cmd, char *bits) {
	if(cmd->bits == -1 && cmd->nreg == -1) {
		return 1;
	}
	
	memcpy(bits, "00000000", 8);
	
	if(cmd->bits != -1) {
		putBits(bits, cmd->bits, 8);
		return 0;
	}

	putBits(bits, cmd->nreg, 4);

	return 0;
}

int getRm(Command *cmd, char *bits) {
	if(cmd->mreg == -1) {
		memcpy(bits, "1111", 4);
		return 0;
	}

	memcpy(bits, "0000", 4);
	putBits(bits, cmd->mreg, 4);
	return 0;
}

int getRd(Command *cmd, char *bits) {
	if(cmd->dreg == -1) {
		memcpy(bits, "1111", 4);
		return 0;
	}

	memcpy(bits, "0000", 4);
	putBits(bits, cmd->dreg, 4);
	return 0;
}

void putBits(char *buf, int n, size_t t) {
	buf = buf + t - 1;
	
	while(n > 0) {
		*buf = (n & 1) + '0';
		n >>= 1;
		buf--;
	}
}
