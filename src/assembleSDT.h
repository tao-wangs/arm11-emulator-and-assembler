#ifndef ASSEMBLESDT_H
#define ASSEMBLESDT_H

#include "utility.h"
#include "assdataprocessing.h"
#include "hash.h"

int32_t assembleSDT(char* instruction, int32_t lastAddress, int32_t pc, hashTable *table);

int32_t assembleLDR(char* mnemonic, char* op1, char* op2, int32_t lastAddress, int32_t pc, hashTable *table);

int32_t assembleSTR(char* mnemonic, char* op1, char* op2);

#endif