#include "assembleSDT.h"
#include <string.h>

int32_t assembleSDT(char* instruction, int32_t lastAddress, int32_t pc, hashTable *table) {

    char *endPtr = instruction;
    char* mnemonic = strtok_r(endPtr, " ,", &endPtr);
    char* op1 = strtok_r(endPtr, " ,", &endPtr);
    char* op2 = strtok_r(endPtr, " ,", &endPtr);

    if (!strcmp(mnemonic, "ldr")) {
        return assembleLDR(mnemonic, op1, op2, lastAddress, pc, table);
    }
    
    return assembleSTR(mnemonic, op1, op2);
}

int32_t assembleLDR(char* mnemonic, char* op1, char* op2, int32_t lastAddress, int32_t pc, hashTable *table) {
    int32_t cond = 9 << 28;
    int32_t filler = 1 << 26;
    int32_t i = 0 << 25;
    int32_t p = 1 << 24;
    int32_t u = 1 << 23;
    int32_t l = 1 << 20;
    int32_t rn = 0;
    int32_t rd = stringToInt(op1) << 12;
    int32_t offset = 0;

    if (op2[0] == '=') {
        if (stringToInt(op2) <= 0xFF) {
            return assembleDataProcessing(strcat("mov ", strcat(op1, strcat(" ", op2))), table);
        } else {
            int32_t expression = stringToInt(op2);
            //put expression at the end of the program
            //use the address of this value with the PC as the base register and a
            //calculated offset (ldr rd,[PC,offset]) where offset = (value address) - [PC];
            //something like fileWrite(expression, fileName);
            offset = lastAddress - pc;
        }
    } else if (strlen(op2) <= 5) {
        rn = stringToInt(removeBrackets(op2));
    } else {
        int32_t index = 0;
        for (int32_t i = 0; i < strlen(op2); i++) {
           if (op2[i] == ']') {
               break;
           }
        }
        char* endPtr;
        if (!(index == 3) && !(index == 4)) {
            rn = stringToInt(strtok_r(removeBrackets(op2), ",", &endPtr)) << 16;
            offset = stringToInt(endPtr);
        } else {
            p = 0 << 24;
            rn = stringToInt(removeBrackets(strtok_r(op2, ",", &endPtr))) << 16;
            offset = stringToInt(endPtr);
        }
    }

    return cond | filler | i | p | u | l | rn | rd | offset;
}

int32_t assembleSTR(char* mnemonic, char* op1, char* op2) {
    int32_t cond = 9 << 28;
    int32_t filler = 1 << 26;
    int32_t i = 0 << 25;
    int32_t p = 1 << 24;
    int32_t u = 1 << 23;
    int32_t l = 0 << 20;
    int32_t rn;
    int32_t rd = stringToInt(op1) << 12;
    int32_t offset;

    if (strlen(op2) <= 5) {
        rn = stringToInt(removeBrackets(op2));
        offset = 0;
    } else {
        int32_t index = 0;
        for (int32_t i = 0; i < strlen(op2); i++) {
           if (op2[i] == ']') {
               break;
           }
        }
        if (!(index == 3) && !(index == 4)) {
            char* endPtr;
            rn = stringToInt(strtok_r(removeBrackets(op2), ",", &endPtr));
            offset = stringToInt(endPtr);
        } else {
            p = 0 << 24;
            char* endPtr;
            rn = stringToInt(removeBrackets(strtok_r(op2, ",", &endPtr)));
            offset = stringToInt(endPtr);
        }
    }

    return cond | filler | i | p | u | l | rn | rd | offset;
}

char *removeBrackets(char *token) {
    return strtok(token, "[]");
}
