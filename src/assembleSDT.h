#ifndef ASSEMBLESDT_H
#define ASSEMBLESDT_H

#include "utility.h"
#include "assdataprocessing.h"
#include "hash.h"

uint32_t assembleSDT(char* instruction, uint32_t lastAddress, uint32_t pc, hashTable *table, uint32_t *values[]);

uint32_t assembleLDR(char* mnemonic, char* op1, char* op2, uint32_t lastAddress, uint32_t pc, hashTable *table, uint32_t *values[]);

uint32_t assembleSTR(char* mnemonic, char* op1, char* op2);

#endif