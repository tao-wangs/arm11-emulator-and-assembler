#include "assembleSDT.h"
#include <string.h>

int32_t assembleSDT(char* instruction, int32_t lastAddress, int32_t pc, hashTable *table, uint32_t values[]) {

    char** tokens = tok(instruction, 3);
    char* mnemonic = tokens[0];
    char* op1 = tokens[1];
    char* op2 = tokens[2];

    if (!strcmp(mnemonic, "ldr")) {
        return assembleLDR(mnemonic, op1, op2, lastAddress, pc, table, values);
    }
    
    return assembleSTR(mnemonic, op1, op2);
}

int32_t assembleLDR(char* mnemonic, char* op1, char* op2, int32_t lastAddress, int32_t pc, hashTable *table, uint32_t values[]) {
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
            char str[512];
            strcat(str, "mov ");
            strcat(str, op1);
            strcat(str, " ");
            strcat(str, op2);
            return assembleDataProcessing(str, table);
        } else {
            uint32_t expression = stringToInt(op2);
            uint32_t arraySize = sizeof(values) / sizeof(values[0]);
            for (int i = 0; i < arraySize; i++) {
                if (values[i] == 0) {
                    values[i] = expression;
                    break;
                }
            }
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
