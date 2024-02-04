#include <stdio.h>
#include <string.h>
#include "pardef.h"
#include "parser.h"

const char * INST_KEYS[] = {
	"ADD",
	"SUB",
	"ORR",
	"AND",
	"LBS",
	"RBS",
	"XOR",
	"NOT",
	"S",
	"L",
	"FLASH",
	"FLUSH",
	"DELAY",
};

int parseDest(const char *dest, Command *cmd) {
	int regnum;
	
	if(dest == NULL) {
		cmd->dreg = 15;
	}
	else if( sscanf(dest, "r%d", &regnum) && regnum <= 15) {
		cmd->dreg = regnum;
	}
	else {
		cmd->dreg = -1;
		strcpy(cmd->dlabel, dest);
	}
	
	return 0;
}

int parseFunc(const char *fstr, Command *cmd) {
	if(fstr == NULL) {
		cmd->f = SET;
		return 0;
	}

	for(int i = 0; i < 13; i++) {
		if( strcmp( fstr, INST_KEYS[i] ) == 0 ) {
			cmd->f = i;
			return 0;
		}
	}

	return 1;
}

int parseArgs(const char *arg1, const char *arg2, Command *cmd) {
	if(arg1 == NULL) {
		cmd->mreg = -1;
		cmd->nreg = -1;
		cmd->bits = 0;
		return 0;
	}

	// 1 arg (set call)
	if(arg2 == NULL) {
		int tmp;
		if( sscanf(arg1, "r%d", &tmp) && tmp <= 15 ) {
			cmd->mreg = tmp;
			cmd->nreg = -1;
			cmd->bits = 0;
			return 0;
		}
		else if( !parseByte(arg1, &tmp) ) {
			cmd->mreg = -1;
			cmd->nreg = -1;
			cmd->bits = tmp;
			return 0;
		}
		else if( sscanf(arg1, "%d", &tmp) && tmp < 256 ){
			cmd->mreg = -1;
			cmd->nreg = -1;
			cmd->bits = tmp;
			return 0;
		}
		
		return 1;
	}

	// 2 arg
	int tmp;

	if( sscanf(arg1, "r%d", &tmp) && tmp <= 15 ) {
		cmd->mreg = tmp;

		// Rn, bit8, or decimal?
		if( sscanf(arg2, "r%d", &tmp) && tmp <= 15 ) {
			if(cmd->f == STORE) {
				cmd->dreg = cmd->mreg;
				cmd->mreg = tmp;
				cmd->nreg = -1;
			}
			else {
				cmd->nreg = tmp;
			}
			cmd->bits = -1;
			return 0;
		}
		else if( !parseByte(arg2, &tmp) ) {
			cmd->nreg = -1;
			cmd->bits = tmp;
			return 0;	
		}
		else if( sscanf(arg2, "%d", &tmp) && tmp < 256 ) {
			cmd->nreg = -1;
			cmd->bits = tmp;
			return 0;
		}

		return 1;
	}
	else {
		// A LOAD call
		//sscanf(arg1, "%d", &(cmd->dreg));
		sscanf(arg2, "r%d", &tmp);

		cmd->mreg = tmp;
		strcpy( cmd->dlabel, arg1 );
		cmd->bits = -1;
		cmd->nreg = -1;
	}

	return 0;
}

// Expected str: b_######## or B_########
int parseByte(const char *bit8, int *bdest) {
	char key0, key1;

	switch(*bit8) {
		case 'b':
			key0 = '0';
			key1 = '1';
			break;
		case 'B':
			key0 = '~';
			key1 = '@';
			break;
		default:
			return 1;
	}

	int byte = 0;

	for(int i = 0; i < 8; i++) {
		char bit = bit8[i + 2];

		if( !(bit == key0 || bit == key1) ) {
			return 2;
		}
		
		byte <<= 1;
		byte |= bit == key1;
	}

	*bdest = byte;
	return 0;
}
