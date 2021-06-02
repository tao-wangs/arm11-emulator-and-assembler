#ifndef DECODE_H
#define DECODE_H

#include <stdbool.h>
#include "arm_state.h"

#define MASK_1 (unsigned int) 0x0C000000
#define MASK_2 (unsigned int) 0x0FC000F0
#define HALT_MASK (unsigned int) 0x00000000

#define N_MASK 0x80000000
#define Z_MASK 0x40000000
#define V_MASK 0x10000000

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
    LSL = 0,
    LSR = 1,
    ASR = 2,
    ROR = 3,
} SHIFT_TYPE;


INSTRUCTION_TYPE decode(uint32_t instruction);
bool conditionMet(uint32_t conditionCode, ARM_STATE *machine);
int32_t toLittleEndian(int32_t word);

#endif
