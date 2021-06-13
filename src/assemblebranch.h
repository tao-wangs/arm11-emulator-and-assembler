#ifndef __ASSEMBLE_BRANCH_H__
#define __ASSEMBLE_BRANCH_H__

#include <stdint.h>

#include "hash.h"

uint32_t assembleBranch(char *instruction, hashTable *instrs, hashTable *labels, uint32_t pc);

#endif
