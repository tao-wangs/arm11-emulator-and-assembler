#include <stdint.h>
#include <stdio.h>

#include "secondpass.h"

char *allOperations[NUM_INSTRS_TOTAL] = {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst",
       	"teq", "cmp", "mul", "mla", "ldr", "str", "beq", "bne", "bge", "blt", 
	"bgt", "ble", "bal", "b", "lsl", "andeq"};

uint64_t typeCodes[NUM_INSTRS_TOTAL] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4}; 

char *branchOperations[NUM_INSTRS_BRANCH] = {"beq", "bne", "bge", "blt", "bgt", "ble", "bal", "b"};
uint64_t branchOpCodes[NUM_INSTRS_BRANCH] = {0, 1, 10, 11, 12, 13, 14, 14};

char *dpOperations[NUM_INSTRS_DP] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov"};
uint64_t dpOpCodes[NUM_INSTRS_DP] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13};

void secondPass(hashTable *labels, char* readfile, char* outfile, uint32_t last_addr){
    hashTable *typeTable = createHashTable(NUM_INSTRS_TOTAL);
    hashTable *dpTable = createHashTable(NUM_INSTRS_DP);
    hashTable *branchTable = createHashTable(NUM_INSTRS_BRANCH);

    addHashList(typeTable, allOperations, typeCodes);
    addHashList(dpTable, dpOperations, dpOpCodes);
    addHashList(branchTable, branchOperations, branchOpCodes);
    
    uint32_t mem_addresses[last_addr];
    for(int i = 0; i < last_addr; i++){
        mem_addresses[i] = 0;    
    }
    
    uint32_t pc = 0;

    FILE *fp = fopen(readfile, "r");
    
    FILE *fpw = fopen(outfile, "w"); //create empty file to write to
    fclose(fpw);

    char buffer[MAX_LINE_SIZE];
    char **mnemonic;
    fgets(buffer, MAX_LINE_SIZE, fp);

    while(!feof(fp)){
        mnemonic = tok(buffer, 1);
	if(!mnemonic[0]){
	    mnemonic[0] = "null";
	}
	uint64_t type = lookupVal(typeTable, mnemonic[0]);
	switch(type){
	    case LAB: break;
            case B: fileWrite(assembleBranch(buffer, branchTable, labels, pc), outfile); break;
            case SDT: fileWrite(assembleSDT(buffer, last_addr, pc, dpTable, mem_addresses), outfile); break; 
            case MUL: fileWrite(assembleMultiply(buffer), outfile); break;
            case SPEC: fileWrite(assembleSpecialInstruction(buffer, dpTable), outfile); break;  
	    case DP: fileWrite(assembleDataProcessing(buffer, dpTable), outfile); break;
            default: printf("instruction not recognised");
	}
	freeTok(mnemonic);
	if(type != LAB){
	    pc += 4;
	} 
        fgets(buffer, MAX_LINE_SIZE, fp);
    }
    
    //fileWrite(0, outfile); //HALT instruction

    for(int i = 0; i < last_addr; i++){
        if(mem_addresses[i] != 0){
	    fileWrite(mem_addresses[i], outfile);	
	}    
    }

    fclose(fp);
}

