#ifndef MULTIPLY_H
#define MULTIPLY_H

#include "arm_state.h"

#define BIT31_MASK 0x80000000

#define COND_MASK 0xF0000000
#define A_MASK 0x00200000
#define S_MASK 0x00100000
#define RD_MASK_MUL 0x000F0000
#define RN_MASK_MUL 0x0000F000
#define RS_MASK 0x00000F00
#define RM_MASK 0x0000000F

void decodeMultiply(uint32_t instruction, ARM_STATE *machinePtr);
void executeMultiply(int32_t Rm, int32_t Rs, int32_t Rd, int32_t S, ARM_STATE *machine);
void executeMultiplyAccumulate(int32_t Rm, int32_t Rs, int32_t Rn, int32_t Rd, int32_t S, ARM_STATE *machine);

#endif
