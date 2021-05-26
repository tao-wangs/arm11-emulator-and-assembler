#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H
#include "arm_state.h"

void decodeSDT(unsigned int instruction, ARM_STATE *machinePtr);
void executeLoad(int P, int U, int Rn, int Rd, int Offset, ARM_STATE *machine);
void executeStore(int P, int U, int Rn, int Rd, int Offset, ARM_STATE *machine);
unsigned int rotateRight(unsigned int value, int shift);
bool conditionMet(unsigned int conditionCode, ARM_STATE *machine);

#endif  