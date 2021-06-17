#ifndef ASSEMBLESDT_H
#define ASSEMBLESDT_H

#include "../utility.h"
#include "tokeniser.h"
#include "assdataprocessing.h"
#include "hash.h"

#define FILLER_SHIFT_SDT 26
#define I_SHIFT 25
#define P_SHIFT 24
#define U_SHIFT 23
#define L_SHIFT 20
#define RD_SHIFT_SDT 12
#define RN_SHIFT_SDT 16

int32_t assembleSDT(char* instruction, int32_t lastAddress, int32_t pc, hashTable *table, uint32_t values[]);

int32_t assembleLDR(char* mnemonic, char* op1, char* op2, int32_t lastAddress, int32_t pc, hashTable *table, uint32_t values[]);

int32_t assembleSTR(char* mnemonic, char* op1, char* op2);

char *removeBrackets(char *token);

char *trim(char *token);

#endif
