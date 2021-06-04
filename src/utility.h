#ifndef UTILITY_H
#define UTILITY_H

#include <stdbool.h>
#include <stdint.h>
#include "emulator/arm_state.h"

#define PC 15
#define CPSR 16

#define INSTRUCTION_SIZE 4

#define ONE_BIT_MASK 0x1
#define TWO_BIT_MASK 0x3
#define FOUR_BIT_MASK 0xF
#define EIGHT_BIT_MASK 0xFF
#define TWO_BYTE_MASK 0XFFFF
#define THREE_BYTE_MASK 0xFFFFFF

#define N_MASK 0x80000000
#define Z_MASK 0x40000000
#define C_MASK 0x20000000
#define V_MASK 0x10000000

#define N_SHIFT 31
#define Z_SHIFT 30
#define C_SHIFT 29
#define V_SHIFT 28

#define CONDCODE_SHIFT 28
#define IMM_OPERAND_SHIFT 25
#define SET_FLAGS_SHIFT 20

#define ONE_BYTE_SHIFT 8
#define TWO_BYTE_SHIFT 16
#define THREE_BYTE_SHIFT 24

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

bool conditionMet(uint32_t conditionCode, ARM_STATE *machine);
int32_t toLittleEndian(int32_t word);
uint32_t rotateRight(uint32_t value, int32_t shift);

#endif
