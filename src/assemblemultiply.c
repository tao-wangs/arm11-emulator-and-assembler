
#include "assemblemultiply.h"

int32_t assembleMultiply(char* instruction) {

    char **tokens = tok(instruction, 5);
    
    char* mnemonic = tokens[0];
    char* op1 = tokens[1];
    char* op2 = tokens[2];
    char* op3 = tokens[3];
    char* op4 = tokens[4];

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
