
#include "assemblemultiply.h"

int32_t assembleMultiply(char* instruction) {

    char **tokens = tok(instruction, 5);
    
    char* mnemonic = tokens[0];
    char* op1 = tokens[1];
    char* op2 = tokens[2];
    char* op3 = tokens[3];
    char* op4 = tokens[4];

    int32_t cond = AL << CONDCODE_SHIFT;
    int32_t a = !strcmp(mnemonic, "mla") << A_SHIFT;
    int32_t s = 0x0 << S_SHIFT;
    int32_t filler = 0x9 << FILLER_SHIFT_MUL;
    
    int32_t rd = stringToInt(op1) << RD_SHIFT_MUL;
    int32_t rm = stringToInt(op2);
    int32_t rs = stringToInt(op3) << RS_SHIFT_MUL;
    int32_t rn = !op4 ? 0 : stringToInt(op4) << RN_SHIFT_MUL;

    return cond | a | s | rd | rn | rs | filler | rm; 
}
