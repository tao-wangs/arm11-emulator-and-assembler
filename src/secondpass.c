#include <stdint.h>
#include <stdio.h>

#include "secondpass.h"
#include "hash.h"
#include "tokeniser.h"
#include "assembleSDT.h"
#include "assemblemultiply.h"
#include "assemblebranch.h"
#include "binfilewriter.h"

typedef enum {LAB, B, SDT, MUL, SPEC, DP} type;

char *allOperations[NUM_INSTRS_TOTAL] = {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst",
       	"teq", "cmp", "mul", "mla", "ldr", "str", "beq", "bne", "bge", "blt", 
	"bgt", "ble", "bal", "b", "lsl", "andeq"};

uint64_t typeCodes[NUM_INSTRS_TOTAL] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4}; 

char *branchOperations[NUM_INSTRS_BRANCH] = {"beq", "bne", "bge", "blt", "bgt", "ble", "bal", "b"};
uint64_t branchOpCodes[NUM_INSTRS_BRANCH] = {0, 1, 10, 11, 12, 13, 14, 14};

char *dpOperations[NUM_INSTRS_DP] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov"};
uint64_t dpOpCodes[NUM_INSTRS_DP] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13};

//TODO: special

void secondPass(hashTable *labels, char* filename, uint32_t last_addr){
    hashTable *typeTable = createHashTable(NUM_INSTRS_TOTAL);
    hashTable *dpTable = createHashTable(NUM_INSTRS_DP);
    hashTable *branchTable = createHashTable(NUM_INSTRS_BRANCH);

    addHashList(typeTable, allOperations, typeCodes);
    addHashList(dpTable, dpOperations, dpOpCodes);
    addHashList(branchTable, branchOperations, branchOpCodes);

    uint32_t pc = 0;
    FILE *fp = fopen(filename, "r");
    char buffer[MAX_LINE_SIZE];
    char **mnemonic;

    while(!feof(fp)){
        fgets(buffer, MAX_LINE_SIZE, fp);
        mnemonic = tok(buffer, 1);
	switch(lookupVal(typeTable, *mnemonic)){
	    case LAB: break;
            case B: fileWrite(assembleBranch(buffer, branchTable, labels, pc), filename); break;
            case SDT: fileWrite(assembleSDT(buffer, last_addr, pc, dpTable), filename); break;
            case MUL: fileWrite(assembleMultiply(buffer), filename); break;
            case SPEC: printf("to be implemented"); break; 
	    case DP: printf("to be implemented"); break;
            default: printf("instruction type not recognised");
	}
    }

    fclose(fp);
    fileWrite(0, filename); //HALT instruction
}

