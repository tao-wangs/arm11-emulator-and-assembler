#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

#include "utility.h"

#define P_SHIFT 24
#define U_SHIFT 23
#define L_SHIFT 20
#define RN_SHIFT_SDT 16
#define RD_SHIFT_SDT 12
#define RS_SHIFT_SDT 4

#define OFFSET_MASK_SDT 0x00000FFF
#define FIVE_BIT_MASK 0x1F

#define SHIFT_SHIFT 4
#define INTEGER_SHIFT 3
#define TYPE_SHIFT 1

void decodeSDT(uint32_t instruction, ARM_STATE *machineptr);
void executeLoad(int32_t p, int32_t u, int32_t rn, int32_t rd, uint32_t offset, ARM_STATE *machine);
void executeStore(int32_t p, int32_t u, int32_t rn, int32_t rd, int32_t offset, ARM_STATE *machine);

#endif
