#ifndef __ASSEMBLE_BRANCH_H__
#define __ASSEMBLE_BRANCH_H__

#include <stdint.h>

#include "hash.h"

#define NUM_INSTRS_BRANCH 8

static char *branchOperations[NUM_INSTRS_BRANCH] = {"beq", "bne", "bge", "blt", "bgt", "ble", "bal", "b"};
static uint64_t branchOpCodes[NUM_INSTRS_BRANCH] = {0, 1, 10, 11, 12, 13, 14, 14};

uint32_t assembleBranch(char *instruction, hashTable *instrs, hashTable *labels, uint32_t pc);

#endif
