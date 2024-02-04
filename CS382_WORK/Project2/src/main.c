#include <stdio.h>
#include <string.h>
#include "pardef.h"
#include "parser.h"
#include "bcode.h"

int parseLine(char *line, Command *cmd) {
	char dest[10];
	char func[10];
	char arg1[10];
	char arg2[10];
	
	int numParsed = sscanf(line, \
			" %[A-Za-z0-9] <- %[A-Za-z0-9_@~](%[A-Za-z0-9], %[bB_r0-9@~])",\
			dest, func, arg1, arg2);	
	
	//printf("%s,%s,%s,%s\n", dest, func, arg1, arg2);
	
	if(numParsed < 2) { return 1; }
	if( parseDest(dest, cmd) ) { return 2; }
	
	if(numParsed == 2) {
		if( strchr(line, (int) '(') == NULL ) {
			if( parseFunc(NULL, cmd) ) { return 3; }
			if( parseArgs(func, NULL, cmd) ) { return 4; }
		}
		else {
			if( parseFunc(func, cmd) ) { return 7; }
			if( parseArgs(NULL, NULL, cmd) ) { return 8; }
		}
	}
	else {
		if( parseFunc(func, cmd) ) { return 5; }
		if( parseArgs(arg1, numParsed == 4 ? arg2 : NULL, cmd) ) { return 6; }
	}
	return 0;
}

int toBitCode(Command *cmd, char *bits) {
	translateLabel(cmd);

	if( getOpCode(cmd, bits) ) { return 1; }
	if( getMidByte(cmd, bits + 8) ) { return 2; }
	if( getRm(cmd, bits + 16) ) { return 3; }
	if( getRd(cmd, bits + 20) ) { return 4; }
	return 0;
}

int toDecimal(char *bits) {
	int res = 0;
	int i = 0;
	while(i < 24) {
		res <<= 1;
		res |= (int) (*bits - '0');
		bits++;
		i++;
	}

	return res;
}

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("Error: No file was inputed.");
		return 1;
	}

	if( strstr(argv[1], ".bld") == NULL ) {
		printf("Error: File is not a .bld file");
		return 1;
	}

	FILE * src = fopen(argv[1], "r");

	if(src == NULL) {
		printf("Error: File not found in current directory");
		return 1;
	}
	
	char fhead[25];
	strncpy( fhead, argv[1], strchr(argv[1], (int) '.') - argv[1] );
	
	char outName[29];
	char dataName[29];

	sprintf(outName, "%s%s", fhead, ".bin");
	sprintf(dataName, "%s%s", fhead, ".dat");
	
	// Remove prexisting output files
	remove(outName);
	remove(dataName);

	FILE * outf = fopen(outName, "wb");
	
	int code = 0;
	int parseMode = 0; // 0 = INST, 1 = DATA
	char buffer[50];
	while( fgets(buffer, 50, src) != NULL ) {
		if(*buffer == '\n')	{ continue; }

		// Parse Toggle?
		if( (!parseMode && strncmp(buffer, "> DATA", 6) == 0) || \
			(parseMode && strncmp(buffer, "> START", 6) == 0) ) {
			
			fclose(outf);
			parseMode = !parseMode;
			outf = fopen( parseMode ? dataName : outName, "wb" );
			continue;
		}
		
		// DATA parse
		if(parseMode) {
			char label[10];
			char inputs[50]; // 50 characters max
			
			printf("DATA PARSING %s", buffer);

			sscanf(buffer, " %[A-Za-z] <- %[0-9 ]", label, inputs);
			
			char * inPos = (char *) inputs;
			int size = 0;
			int tmp;
			
			while( sscanf(inPos, "%d", &tmp) ) {
				// Values are only 1 byte
				fwrite(&tmp, 1, 1, outf);
				size++;

				// Move input ptr
				char * sep = strchr(inPos, (int) ' ');
				if(sep == NULL) { break; }

				inPos = sep + 1;
			}

			if( putLabel(label, size) ) {
				printf("***DATA PUT ERROR***\n");
				code = 3;
				break;
			}

			continue;
		}
		
		// INST parse
		printf("PARSING %s", buffer);
		
		char res[24];
		Command cmd;
		memset(cmd.dlabel, 0, 10); // Reset cmd label
		
		int pres = parseLine(buffer, &cmd);
		//printf("Parse Status: %d\n", pres);
		if(pres) { 
			printf("***PARSE ERROR***\n");
			code = 1;
			break; 
		}

		//printf("Cmd Data\n");
		//printf("f: %d\n", cmd.f);
		//printf("dreg: %d\n", cmd.dreg);
		//printf("dlab: %s\n", cmd.dlabel);
		//printf("mreg: %d\n", cmd.mreg);
		//printf("nreg: %d\n", cmd.nreg);
		//printf("bits: %d\n", cmd.bits);	

		int bres = toBitCode(&cmd, res);
		//printf("Bit Status: %d\n", bres);
		if(bres) { 
			printf("***BITCODE ERROR***\n");
			code = 2;
			break;	
		}

		//printf("%s\n", res);
		
		int dec = toDecimal(res);
		//printf("DECIMAL: %d, HEX: %x\n", dec, dec);
		
		fwrite(&dec, 3, 1, outf);
		//fprintf(outf, "%s\n", res);
	}

	if(code == 0) {
		printf("COMPILATION SUCCESSFUL\n");
	}
	
	fclose(src);
	fclose(outf);
	return code;
}
