#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "utility.h"
#include "binfilewriter.h"

void fileWrite(uint32_t instr, char* filename) {

	FILE *fptr;
	fptr = fopen(filename, "ab");

	if (!fptr) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	char *instrString = malloc(INSTRUCTION_BITS * sizeof(char));
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
