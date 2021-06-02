#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

#include "arm_state.h"

#define COND_MASK 0xF0000000
#define I_MASK 0x02000000
#define P_MASK 0x01000000
#define U_MASK 0x00800000
#define L_MASK 0x00100000
#define RN_MASK 0x000F0000
#define RD_MASK 0x0000F000
#define OFFSET_MASK 0x00000FFF

#define SHIFT_MASK 0x00000FF0
#define RM_MASK 0x0000000F

void decodeSDT(uint32_t instruction, ARM_STATE *machinePtr);
void executeLoad(int32_t P, int32_t U, int32_t Rn, int32_t Rd, uint32_t Offset, ARM_STATE *machine);
void executeStore(int32_t P, int32_t U, int32_t Rn, int32_t Rd, int32_t Offset, ARM_STATE *machine);
uint32_t rotateRightSDT(uint32_t value, int32_t shift);

#endif
