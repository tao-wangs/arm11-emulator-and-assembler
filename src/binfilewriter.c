#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utility.c"
#include "emulator/binreader.c"

void fileWrite(uint32_t instr, char* filename) {

	FILE *fptr;
	fptr = fopen(filename, "wb");

	if (!fptr) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}


	char *instrString = malloc(32 * sizeof(char));
	if (!instrString) {
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	uint32_t lEndInstr = toLittleEndian(instr);
	
	if (fputs(instrString, fptr) <= 0) {
		perror("Error writing to file");
		exit(EXIT_FAILURE);
	}

	fclose(fptr);
}
