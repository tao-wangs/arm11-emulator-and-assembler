#include "assembleSDT.h"
#include <string.h>
#include <ctype.h>

int32_t assembleSDT(char* instruction, int32_t lastAddress, int32_t pc, hashTable *table, uint32_t values[]) {

    printf("Checkpoint\n");

    char** tokens = tok(instruction, 3);
    char* mnemonic = tokens[0];
    char* op1 = tokens[1];
    char* op2 = tokens[2];

    //disgusting code, sorry
    char temp[strlen(instruction)];
    strcpy(temp, instruction);
    char *strPtr = temp;
    for (int i = 0; i < 2; i++) {
        op2 = strtok_r(strPtr, " ,", &strPtr);
    }
    op2 = strPtr;
    //ends here

    if (!strcmp(mnemonic, "ldr")) {
        return assembleLDR(mnemonic, op1, op2, lastAddress, pc, table, values);
    }
    
    assert(!strcmp(mnemonic, "str"));
    return assembleSTR(mnemonic, op1, op2);
}

int32_t assembleLDR(char* mnemonic, char* op1, char* op2, int32_t lastAddress, int32_t pc, hashTable *table, uint32_t values[]) {
    int32_t cond = 14 << CONDCODE_SHIFT;
    int32_t filler = 1 << FILLER_SHIFT_SDT;
    int32_t i = 0 << I_SHIFT;
    int32_t p = 1 << P_SHIFT;
    int32_t u = 1 << U_SHIFT;
    int32_t l = 1 << L_SHIFT;
    int32_t rn = 0;
    int32_t rd = stringToInt(op1) << RD_SHIFT_SDT;
    int32_t offset = 0;

    if (op2[0] == '=') {
        if (stringToInt(op2) <= 0xFF) {
            char str[512] = {0};
            strcat(str, "mov ");
            strcat(str, op1);
            strcat(str, ",");
            strcat(str, op2);
            return assembleDataProcessing(str, table);
        } else {
            uint32_t expression = stringToInt(op2);
            uint32_t arraySize = sizeof(values) / sizeof(values[0]);
            for (int i = 0; i < arraySize; i++) {
                if (values[i] == 0) {
                    values[i] = expression;
                    break;
                } else {
                    lastAddress += 4;
                }
            }
            rn = 0xF << RN_SHIFT_SDT;

            offset = lastAddress - (pc + 8);

        }
    } else if (strlen(op2) <= 5) {
        rn = stringToInt(removeBrackets(op2)) << RN_SHIFT_SDT;
    } else {
        int32_t index = 0;
        for (int32_t i = 0; i < strlen(op2); i++) {
           if (op2[i] == ']') {
               break;
           }
           index++;
        }
        char* endPtr;
        if (!(index == 3) && !(index == 4)) {
            rn = stringToInt(strtok_r(removeBrackets(op2), ",", &endPtr)) << RN_SHIFT_SDT;
            if (stringToInt(endPtr) < 0) {
                u = 0 << U_SHIFT;
            }
            offset = abs(stringToInt(endPtr));
        } else {
            p = 0 << P_SHIFT;
            rn = stringToInt(removeBrackets(strtok_r(op2, ",", &endPtr))) << RN_SHIFT_SDT;
            offset = stringToInt(endPtr);
        }
    }

    return cond | filler | i | p | u | l | rn | rd | offset;
}

int32_t assembleSTR(char* mnemonic, char* op1, char* op2) {
    int32_t cond = 14 << CONDCODE_SHIFT;
    int32_t filler = 1 << FILLER_SHIFT_SDT;
    int32_t i = 0 << I_SHIFT;
    int32_t p = 1 << P_SHIFT;
    int32_t u = 1 << U_SHIFT;
    int32_t l = 0 << L_SHIFT;
    int32_t rn = 0;
    int32_t rd = stringToInt(op1) << RD_SHIFT_SDT;
    int32_t offset = 0;

    if (strlen(op2) <= 5) {
        rn = stringToInt(removeBrackets(op2)) << RN_SHIFT_SDT;
    } else {
        int32_t index = 0;
        for (int32_t i = 0; i < strlen(op2); i++) {
           if (op2[i] == ']') {
               break;
           }
           index++;
        }
        char* endPtr;
        if (!(index == 3) && !(index == 4)) {
            int cnt = 0;
            for (int i = 0; i < strlen(op2); i++) {
                if (op2[i] == 'r') {
                    cnt++;
                }
            }
            if (cnt == 2) {
                i = 1 << I_SHIFT;
            }
            rn = stringToInt(strtok_r(removeBrackets(trim(op2)), " ,", &endPtr)) << RN_SHIFT_SDT;
            offset = stringToInt(endPtr);
        } else {
            if (op2[index + 1] == ',') {
                p = 0 << P_SHIFT;
                i = 1 << I_SHIFT;
                rn = stringToInt(removeBrackets(strtok_r(op2, " ,", &endPtr))) << RN_SHIFT_SDT;
                offset = stringToInt(endPtr);
            } else {
                p = 0 << 24;
                rn = stringToInt(removeBrackets(strtok_r(op2, " ,", &endPtr))) << RN_SHIFT_SDT;
                offset = stringToInt(endPtr);
            }
        }
    }

    return cond | filler | i | p | u | l | rn | rd | offset;
}

char *removeBrackets(char *token) {
    return strtok(token, "[]");
}

char *trim(char *str)
{
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)
    return str;

  
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  end[1] = '\0';

  return str;
}
