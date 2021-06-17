#ifndef DECODE_H
#define DECODE_H

#include "utility.h"

#define TEMP_SHIFT 26
#define TEMP2_SHIFT 4
#define MASK_2 (unsigned int) 0x0FC000F0
#define HALT_MASK (unsigned int) 0x00000000

typedef enum  {
    DataProcessing,
    Multiply,
    SingleDataTransfer,
    Branch,
    Halt
} INSTRUCTION_TYPE;

INSTRUCTION_TYPE decode(uint32_t instruction);

#endif
