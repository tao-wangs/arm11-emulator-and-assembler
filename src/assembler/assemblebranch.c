#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "assemblebranch.h"

uint32_t assembleBranch(char* instruction, hashTable *instrs, hashTable *labels, uint32_t pc){
    char **tokens = tok(instruction, 2);
    char* mnemonic = tokens[0];
    char* label = tokens[1];
    
    uint32_t opcode = (uint32_t) lookupVal(instrs, mnemonic);
    opcode <<= CONDCODE_SHIFT;

    uint32_t addr;

    if(atoi(label) == 0){
        addr = (uint32_t) lookupVal(labels, label); 
    } else {
        addr = atoi(label);
    }

    int32_t offset = addr - (pc + 8);
    offset >>= OFFSET_SHIFT_BRANCH;
    offset &= OFFSET_BIT_MASK;
    freeTok(tokens);
    return (opcode | FILLER_BIT_MASK | offset); 
}
