#ifndef __ASSEMBLE_BRANCH_H__
#define __ASSEMBLE_BRANCH_H__

#include <stdint.h>
#include "hash.h"
#include "tokeniser.h"
#include "utility.h"

#define OFFSET_BIT_MASK 0x00FFFFFF
#define FILLER_BIT_MASK 0x0A000000

#define OFFSET_SHIFT_BRANCH 2

uint32_t assembleBranch(char *instruction, hashTable *instrs, hashTable *labels, uint32_t pc);

#endif
