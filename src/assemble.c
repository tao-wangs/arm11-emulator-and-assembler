#include <stdlib.h>
#include "utility.h"
#include "hash.h"
    
int main(int argc, char **argv) {
    hashTable *hTable = createHashTable(16);
    char *operations[16] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov", "eq", "ne", "ge", "lt","gt","le"};
    uint32_t opcodes[16] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13, 0, 1, 10, 11, 12, 13};
    
    addHashList(hTable, operations, opcodes);    

    printf("%u\n", lookupVal(hTable, "tst"));    
    freeHashTable(hTable);
    return EXIT_SUCCESS;
}

