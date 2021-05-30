#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

#include "arm_state.h"

void decodeSDT(unsigned int instruction, ARM_STATE *machinePtr);
void executeLoad(int P, int U, int Rn, int Rd, unsigned int Offset, ARM_STATE *machine);
void executeStore(int P, int U, int Rn, int Rd, int Offset, ARM_STATE *machine);
unsigned int rotateRightSDT(unsigned int value, int shift);

#endif  