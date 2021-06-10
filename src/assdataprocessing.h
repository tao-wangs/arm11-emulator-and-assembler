#ifndef __ASSEMBLEDATAPROCESSING__
#define __ASSEMBLEDATAPROCESSING__

#define ONE_BYTE_MAX_INT 255
#define ONE_NIBBLE_MAX_INT 15

int32_t generate8BitImmediate(char *operand2);
int32_t undoRotation(int32_t immOperand);

#endif 
