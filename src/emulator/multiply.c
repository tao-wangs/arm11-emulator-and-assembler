#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "multiply.h"
#include "../utility.h"

void decodeMultiply(uint32_t instruction, ARM_STATE *machinePtr) {

    int32_t cond = (instruction >> CONDCODE_SHIFT) & FOUR_BIT_MASK;
    int32_t a = (instruction >> A_SHIFT) & ONE_BIT_MASK;
    int32_t s = (instruction >> SET_FLAGS_SHIFT) & ONE_BIT_MASK;
    int32_t rd = (instruction >> RD_SHIFT_MUL) & FOUR_BIT_MASK;
    int32_t rn = (instruction >> RN_SHIFT_MUL) & FOUR_BIT_MASK;
    int32_t rs = (instruction >> RS_SHIFT_MUL) & FOUR_BIT_MASK;
    int32_t rm = (instruction & FOUR_BIT_MASK);

    if (!conditionMet(cond, machinePtr)) {
        return;
    }

    if (a == 1) {
        executeMultiplyAccumulate(rm, rs, rn, rd, s, machinePtr);
        return;
    }

    executeMultiply(rm, rs, rd, s, machinePtr);
}

void executeMultiply(int32_t rm, int32_t rs, int32_t rd, int32_t s, ARM_STATE *machine) {
    int32_t operand1 = machine->registers[rm];
    int32_t operand2 = machine->registers[rs];

    int32_t result = operand1 * operand2;

    machine->registers[rd] = result;

    if (s == 1) {
        int32_t bit31 = (uint32_t) (result >> BIT31_SHIFT);

        if (result == 0) {
            machine->registers[CPSR] |= Z_MASK;
        }

        if (bit31 == 1) {
            machine->registers[CPSR] |= N_MASK;
            return;
        }

        machine->registers[CPSR] &= ~N_MASK;
    }
}

void executeMultiplyAccumulate(int32_t rm, int32_t rs, int32_t rn, int32_t rd, int32_t s, ARM_STATE *machine) {
    int32_t operand1 = machine->registers[rm];
    int32_t operand2 = machine->registers[rs];
    int32_t operand3 = machine->registers[rn];

    int32_t result = operand1 * operand2 + operand3;
    machine->registers[rd] = result;	

    if (s == 1) {
        int32_t bit31 = (uint32_t) ((result >> BIT31_SHIFT) & ONE_BIT_MASK);

        if (result == 0) {
            machine->registers[CPSR] |= Z_MASK;
        }

        if (bit31 == 1) {
            machine->registers[CPSR] |= N_MASK;
            return;
        }

        machine->registers[CPSR] &= ~N_MASK;
    }
}
