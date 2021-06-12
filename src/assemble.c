#include <stdlib.h>
#include "utility.h"
#include "hash.h"
    
hashTable *createDataProcessingTable(void) {

	hashTable *table = createHashTable(10);

	char *operands[10] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov"};
	uint32_t opcodes[10] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13};

	if(!addHashList(table, operands, opcodes)) {
		perror("The symbol table for dataprocessing could not be made");
		exit(EXIT_FAILURE);
	}

	return table;
}

int main(int argc, char **argv) {
	
	hashTable *dataprocessingTable = createDataProcessingTable();
	
	return EXIT_SUCCESS;
}
