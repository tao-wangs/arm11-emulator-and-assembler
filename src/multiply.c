#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "decode.h"
#include "multiply.h"

void decodeMultiply(uint32_t instruction, ARM_STATE *machinePtr) {

    int32_t Cond = (instruction & COND_MASK) >> 28;
    int32_t A = (instruction & A_MASK) >> 21;
    int32_t S = (instruction & S_MASK) >> 20;
    int32_t Rd = (instruction & RD_MASK_MUL) >> 16;
    int32_t Rn = (instruction & RN_MASK_MUL) >> 12;
    int32_t Rs = (instruction & RS_MASK) >> 8;
    int32_t Rm = (instruction & RM_MASK);

    if (!conditionMet(Cond, machinePtr)) {
        return;
    }

    if (A == 1) {
        executeMultiplyAccumulate(Rm, Rs, Rn, Rd, S, machinePtr);
        return;
    }

    executeMultiply(Rm, Rs, Rd, S, machinePtr);
}

void executeMultiply(int32_t Rm, int32_t Rs, int32_t Rd, int32_t S, ARM_STATE *machine) {
    int32_t operand1 = machine->registers[Rm];
    int32_t operand2 = machine->registers[Rs];

    int32_t result = operand1 * operand2;

    machine->registers[Rd] = result;

    if (S == 1) {
        int32_t bit31 = ((uint32_t) result & BIT31_MASK) >> 31;

        if (result == 0) {
            machine->registers[CPSR] |= Z_MASK;
        }

        if (bit31 == 1) {
            machine->registers[CPSR] |= BIT31_MASK;
            return;
        }

        machine->registers[CPSR] &= ~BIT31_MASK;
    }
}

void executeMultiplyAccumulate(int32_t Rm, int32_t Rs, int32_t Rn, int32_t Rd, int32_t S, ARM_STATE *machine) {
    int32_t operand1 = machine->registers[Rm];
    int32_t operand2 = machine->registers[Rs];
    int32_t operand3 = machine->registers[Rn];

    int32_t result = operand1 * operand2 + operand3;
    machine->registers[Rd] = result;	

    if (S == 1) {
        int32_t bit31 = ((unsigned int) result & BIT31_MASK) >> 31;

        if (result == 0) {
            machine->registers[CPSR] |= Z_MASK;
        }

        if (bit31 == 1) {
            machine->registers[CPSR] |= BIT31_MASK;
            return;
        }

        machine->registers[CPSR] &= ~BIT31_MASK;
    }
}
