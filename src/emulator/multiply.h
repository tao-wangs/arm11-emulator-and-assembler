#ifndef MULTIPLY_H
#define MULTIPLY_H

#include "../utility.h"

#define BIT31_SHIFT 31

#define A_SHIFT 21
#define RD_SHIFT_MUL 16
#define RN_SHIFT_MUL 12
#define RS_SHIFT_MUL 8

void decodeMultiply(uint32_t instruction, ARM_STATE *machinePtr);
void executeMultiply(int32_t rm, int32_t rs, int32_t rd, int32_t s, ARM_STATE *machine);
void executeMultiplyAccumulate(int32_t rm, int32_t rs, int32_t rn, int32_t rd, int32_t s, ARM_STATE *machine);

#endif
