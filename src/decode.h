#ifndef DECODE_H
#define DECODE_H

#include <stdbool.h>
#include "arm_state.h"

#define nMask 0x80000000
#define zMask 0x40000000
#define vMask 0x10000000

typedef enum  {
    DataProcessing,
    Multiply,
    SingleDataTransfer,
    Branch,
    Halt
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
