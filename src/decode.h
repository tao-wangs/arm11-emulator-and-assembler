#ifndef DECODE_H
#define DECODE_H

#include <stdbool.h>
#include "arm_state.h"

typedef enum  {
    DataProcessing,
    Multiply,
    SingleDataTransfer,
    Branch
} INSTRUCTION_TYPE;

typedef enum {
    EQ = 0x0,
    NE = 0x1,
    GE = 0xA, 
    LT = 0xB,
    GT = 0xC, 
    LE = 0xD, 
    AL = 0xE
} CONDITION_CODE;

typedef enum {
    LSL,
    LSR,
    ASR,
    ROR,
} SHIFT_TYPE;


INSTRUCTION_TYPE decode(unsigned int instruction);
bool conditionMet(unsigned int conditionCode, ARM_STATE *machine);

#endif
