#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "hash.h"
#include "tokeniser.h"
#include "assembleSDT.h"
#include "assemblemultiply.h"
#include "assemblebranch.h"
#include "binfilewriter.h"

#define NUM_INSTRS_TOTAL 24
#define NUM_INSTRS_BRANCH 8
#define NUM_INSTRS_DP 10
#define MAX_LINE_SIZE 511

typedef enum {LAB, B, SDT, MUL, SPEC, DP} type;

void secondPass(hashTable *labels, char* readfile, char* outfile, uint32_t last_addr);

#endif
