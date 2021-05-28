#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "decode.h"
#include "multiply.h"

void decodeMultiply(unsigned int instruction, ARM_STATE *machinePtr) {
    int condMask = 0xF0000000;
    int aMask = 0x00200000;
    int sMask = 0x00100000;
    int rdMask = 0x000F0000;
    int rnMask = 0x0000F000;
    int rsMask = 0x00000F00;
    int rmMask = 0x0000000F;

    int Cond = (instruction & condMask) >> 28;
    int A = (instruction & aMask) >> 21;
    int S = (instruction & sMask) >> 20;
    int Rd = (instruction & rdMask) >> 16;
    int Rn = (instruction & rnMask) >> 12;
    int Rs = (instruction & rsMask) >> 8;
    int Rm = (instruction & rmMask);

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

    int zMask = 0x4000000;
    int bit31Mask = 0x80000000;

    if (S == 1) {
        int bit31 = ((unsigned int) result & bit31Mask) >> 31;
        
        if (result == 0) {
            machine->registers[CPSR] |= zMask;
        }
        
        if (bit31 == 1) {
            machine->registers[CPSR] |= bit31Mask;
            return;
        }

        machine->registers[CPSR] &= ~bit31Mask;
    }  
}

void executeMultiplyAccumulate(int Rm, int Rs, int Rn, int Rd, int S, ARM_STATE *machine) {
    int operand1 = machine->registers[Rm];
    int operand2 = machine->registers[Rs];
    int operand3 = machine->registers[Rn];

    int result = operand1 * operand2 + operand3;

    int zMask = 0x4000000;
    int bit31Mask = 0x80000000;

    if (S == 1) {
        int bit31 = ((unsigned int) result & bit31Mask) >> 31;
        
        if (result == 0) {
            machine->registers[CPSR] |= zMask;
        }
        
        if (bit31 == 1) {
            machine->registers[CPSR] |= bit31Mask;
            return;
        }

        machine->registers[CPSR] &= ~bit31Mask;
    }  
}
