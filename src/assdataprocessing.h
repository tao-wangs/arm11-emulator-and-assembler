#ifndef ASSEMBLEDATAPROCESSING_H
#define ASSEMBLEDATAPROCESSING_H

#define ONE_BYTE_MAX_INT 255
#define ONE_NIBBLE_MAX_INT 15

#define OPCODE_SHIFT 21
#define FILLER_SHIFT 26
#define RN_SHIFT 16
#define RD_SHIFT 12
#define ROTATE_AMT_SHIFT 8 

int32_t assembleDataProcessing(char *instrString, hashTable *table);
int32_t generate8BitImmediate(char *operand2);
int32_t undoRotation(int32_t immOperand);

#endif 
