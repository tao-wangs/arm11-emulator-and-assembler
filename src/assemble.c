#include <stdlib.h>
#include "utility.h"
#include "hash.h"

#include "firstpass.h"
#include "secondpass.h"

int main(int argc, char **argv) {
    
    /* 
    hashTable *hTable = createHashTable(10);
    char *operations[16] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov", "eq", "ne", "ge", "lt","gt","le"};
    uint64_t opcodes[16] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13, 0, 1, 10, 11, 12, 13};

hashTable *createDataProcessingTable(void) {

	hashTable *hTable = createHashTable(10);

	char *operands[10] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov"};
	uint64_t opcodes[10] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13};

	addHashList(hTable, operands, opcodes);
    printf("%lu\n", lookupVal(hTable, "orr"));   
    freeHashTable(hTable);
    */ 

    long int max_labels = fSize(argv[1]);
    hashTable *labels = createHashTable(max_labels);
    uint32_t last_addr = firstPass(labels, argv[1]);
    
    secondPass(labels, argv[1], last_addr);

        
    return EXIT_SUCCESS;
}
