#include <stdlib.h>
#include "utility.h"
#include "hash.h"

#include "firstpass.h"
#include "assemblebranch.h"

int main(int argc, char **argv) {
    hashTable *assemblyInstructions = createHashTable(NUM_INSTRS_BRANCH);
    addHashList(assemblyInstructions, branchOperations, branchOpCodes);
    /* 
    hashTable *hTable = createHashTable(10);
    char *operations[16] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov", "eq", "ne", "ge", "lt","gt","le"};
    uint64_t opcodes[16] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13, 0, 1, 10, 11, 12, 13};
    
hashTable *createDataProcessingTable(void) {

	hashTable *table = createHashTable(10);

	char *operands[10] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov"};
	uint32_t opcodes[10] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13};

    printf("%lu\n", lookupVal(hTable, "tst"));    
    freeHashTable(hTable);
    */

    long int max_labels = fSize(argv[1]);
    hashTable *labels = createHashTable(max_labels);
    
        
    return EXIT_SUCCESS;
}
