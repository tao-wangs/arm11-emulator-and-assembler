#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "binfilewriter.h"

void fileWrite(uint32_t instr, char* filename) {

	FILE *fptr;
	fptr = fopen(filename, "a");

	if (!fptr) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	
	if (fwrite((const void *) &instr, sizeof(uint32_t), 1, fptr) <= 0) {
		perror("Error writing to file");
		exit(EXIT_FAILURE);
	}

	fclose(fptr);
}
