#ifndef ASSEMBLEDATAPROCESSING_H
#define ASSEMBLEDATAPROCESSING_H

#include "hash.h"
#include "tokeniser.h"
#include "utility.h"

#define ONE_BYTE_MAX_INT 255
#define ONE_NIBBLE_MAX_INT 15
#define FIVE_BIT_MAX_INT 31

#define OPCODE_SHIFT 21
#define FILLER_SHIFT 26
#define RN_SHIFT 16
#define RD_SHIFT 12
#define ROTATE_AMT_SHIFT 8
#define SHIFT_VALUE_SHIFT 7

uint32_t assembleSpecialInstruction(char *instrString, hashTable *table);
uint32_t assembleDataProcessing(char *instrString, hashTable *table);
bool operandIsConstant(char *immOperandToken);
int32_t generate8BitImmediate(char *operand2);
int32_t undoRotation(int32_t immOperand);

#endif 
