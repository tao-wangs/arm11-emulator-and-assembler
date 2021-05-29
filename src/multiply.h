#ifndef MULTIPLY_H
#define MULTIPLY_H
#include "arm_state.h"

#define bit31Mask 0x80000000

void decodeMultiply(unsigned int instruction, ARM_STATE *machinePtr);
void executeMultiply(int Rm, int Rs, int Rd, int S, ARM_STATE *machine);
void executeMultiplyAccumulate(int Rm, int Rs, int Rn, int Rd, int S, ARM_STATE *machine);

#endif
