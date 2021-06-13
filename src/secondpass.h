#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "hash.h"

#define NUM_INSTRS_TOTAL 24
#define NUM_INSTRS_BRANCH 8
#define NUM_INSTRS_DP 10
#define MAX_LINE_SIZE 511

void secondPass(hashTable *labels, char* filename, uint32_t last_addr);

#endif
