#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "decode.h"
#include "multiply.h"

void decodeMultiply(unsigned int instruction, ARM_STATE *machinePtr) {

    int Cond = (instruction & COND_MASK) >> 28;
    int A = (instruction & A_MASK) >> 21;
    int S = (instruction & S_MASK) >> 20;
    int Rd = (instruction & RD_MASK) >> 16;
    int Rn = (instruction & RN_MASK) >> 12;
    int Rs = (instruction & RS_MASK) >> 8;
    int Rm = (instruction & RM_MASK);

    if (!conditionMet(Cond, machinePtr)) {
        return;
    }

    if (A == 1) {
        executeMultiplyAccumulate(Rm, Rs, Rn, Rd, S, machinePtr);
        return;
    }

    executeMultiply(Rm, Rs, Rd, S, machinePtr);
}

void executeMultiply(int Rm, int Rs, int Rd, int S, ARM_STATE *machine) {
    int operand1 = machine->registers[Rm];
    int operand2 = machine->registers[Rs];

    int result = operand1 * operand2;

    machine->registers[Rd] = result;

    if (S == 1) {
        int bit31 = ((unsigned int) result & BIT31_MASK) >> 31;
        
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

void executeMultiplyAccumulate(int Rm, int Rs, int Rn, int Rd, int S, ARM_STATE *machine) {
    int operand1 = machine->registers[Rm];
    int operand2 = machine->registers[Rs];
    int operand3 = machine->registers[Rn];

    int result = operand1 * operand2 + operand3;

    if (S == 1) {
        int bit31 = ((unsigned int) result & BIT31_MASK) >> 31;
        
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
