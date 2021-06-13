
#include "assemblemultiply.h"

int32_t assembleMultiply(char* instruction) {

    char* endPtr = instruction;
    
    char* mnemonic = strtok_r(endPtr, " ,", &endPtr);
    char* op1 = strtok_r(endPtr, " ,", &endPtr);
    char* op2 = strtok_r(endPtr, " ,", &endPtr);
    char* op3 = strtok_r(endPtr, " ,", &endPtr);
    char* op4 = strtok_r(endPtr, " ,", &endPtr);

    int32_t cond = 0xE << 28;
    int32_t a = !strcmp(mnemonic, "mla") << 21;
    int32_t s = 0x0 << 20;
    int32_t filler = 0x9 << 4;
    
    int32_t rd = stringToInt(op1) << 16;
    int32_t rm = stringToInt(op2);
    int32_t rs = stringToInt(op3) << 8;
    int32_t rn = !op4 ? 0 : stringToInt(op4) << 12;

    return cond | a | s | rd | rn | rs | filler | rm; 
}
