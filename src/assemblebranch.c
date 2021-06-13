#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "assemblebranch.h"
#include "hash.h"

uint32_t assembleBranch(char* instruction, hashTable *instrs, hashTable *labels, uint32_t pc){
    uint32_t offsetBitMask = 0x3FFFFFF;
    uint32_t instrIDBits = 0xA000000;
    char* tokref;
    
    char* mnemonic = strtok_r(instruction, " ", &tokref);
    char* label = strtok_r(NULL, " ", &tokref);

    uint32_t opcode = (uint32_t) lookupVal(instrs, mnemonic);
    opcode <<= 28;

    uint32_t addr;

    if(atoi(label) == 0){
        addr = (uint32_t) lookupVal(labels, label); 
    } else {
        addr = atoi(label);
    }

    int32_t offset = addr - (pc + 8);
    offset &= offsetBitMask;
    offset >>= 2;
    
    return (opcode | instrIDBits | offset); 
}