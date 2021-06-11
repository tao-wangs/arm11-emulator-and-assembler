
#include "assembleSDT.h"

int32_t assembleSDT(char* mnemonic, char* op1, char* op2) {

    int32_t Cond = 0xE << 28;
    int32_t filler = 1 << 26;
    int32_t I;
    int32_t P;
    int32_t U;
    int32_t L = !strcmp(mnemonic, "ldr") << 20;
    int32_t Rn;
    int32_t Rd = stringToInt(op1) << 12;
    int32_t Offset;
    
    if (!strcmp(op2[0], "=") && !strcmp(mnemonic, "ldr")) { //A numeric constant of the form: <=expression> (ldr only).
        if (stringToInt(op2) <= 0xFF) {
            return assembleDataProcessing("mov", op1, NULL, op2);
        } else {
            //second bullet point in examples
        }
    } else if (!strcmp(op2[0], "[")) { //do this later 

    }

    return Cond | filler | I | P | U | L | Rn | Rd | Offset;
}