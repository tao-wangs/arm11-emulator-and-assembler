#ifndef BRANCH_H
#define BRANCH_H

#include "arm_state.h"

#define COND_MASK 0xF0000000
#define OFF_MASK 0x00FFFFFF
#define SIGNED_MASK 0x00800000

int executeBranch(unsigned int instr, ARM_STATE *state);

#endif

