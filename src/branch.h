#ifndef BRANCH_H
#define BRANCH_H

#include "arm_state.h"

#define OFFSET_MASK_BRANCH 0x00FFFFFF
#define SIGN_EXTEND_MASK 0xFC000000

#define SIGN_SHIFT 23

int32_t executeBranch(uint32_t instr, ARM_STATE *state);

#endif

