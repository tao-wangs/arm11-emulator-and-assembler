
#include "assemblemultiply.h"

int32_t assembleMultiply(char *mnemonic, char *op1, char *op2, char *op3, char *op4) {

    int32_t cond = 0xE << 28;
    int32_t a = 0x0 << 21; 
    int32_t s = 0x0 << 20;
    int32_t filler = 0x9 << 4;
    
    int32_t rd = decodeOperand(op1) << 16;
    int32_t rm = decodeOperand(op2);
    int32_t rs = decodeOperand(op3) << 8;
    int32_t rn = decodeOperand(op4) << 12;

    if (!strcmp(mnemonic, "mla")) {
        a = 0x1 << 21;

        return cond | a | s | rd | rn | rs | filler | rm; 
    }

    return cond | a | s | rd | rs | filler | rm; 
}
