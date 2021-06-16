#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "assemblebranch.h"
#include "hash.h"
#include "tokeniser.h"

uint32_t assembleBranch(char* instruction, hashTable *instrs, hashTable *labels, uint32_t pc){
    uint32_t offsetBitMask = 0x00FFFFFF;
    uint32_t instrIDBits = 0x0A000000;
        
    char **tokens = tok(instruction, 2);
    char* mnemonic = tokens[0];
    char* label = tokens[1];
    
    uint32_t opcode = (uint32_t) lookupVal(instrs, mnemonic);
    opcode <<= 28;

    uint32_t addr;

    if(atoi(label) == 0){
        addr = (uint32_t) lookupVal(labels, label); 
    } else {
        addr = atoi(label);
    }

    int32_t offset = addr - (pc + 8);
    offset >>= 2;
    offset &= offsetBitMask;
    freeTok(tokens);
    return (opcode | instrIDBits | offset); 
}
