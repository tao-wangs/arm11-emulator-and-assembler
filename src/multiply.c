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
    int Rn = (instruction & rnMask);
    int Rs = (instruction & rsMask) >> 8;
    int Rm = (instruction & rmMask) >> 12;


    if (!conditionMet(Cond, machinePtr)) {
        return;
    }

    if (A == 1) {
        executeMultiplyAccumulate(Rm, Rs, Rn, Rd, S, machinePtr);
    }

    executeMultiply(Rm, Rs, Rd, S, machinePtr);
}

void executeMultiply(int Rm, int Rs, int Rd, int S, ARM_STATE *machine) {
    int operand1 = machine->registers[Rm];
    int operand2 = machine->registers[Rs];

    int result = operand1 * operand2;

    machine->registers[Rd] = result;

    int zMask = 0x4000000;

    if (S == 1) {
        if (result == 0) {
            
            machine->registers[CPSR] |= zMask;
        }
        machine->registers[CPSR] |= (result >> (WORD_SIZE - 1));
    }   
}

void executeMultiplyAccumulate(int Rm, int Rs, int Rn, int Rd, int S, ARM_STATE *machine) {
    int operand1 = machine->registers[Rm];
    int operand2 = machine->registers[Rs];
    int operand3 = machine->registers[Rn];

    int result = operand1 * operand2 + operand3;

    int zMask = 0x4000000;

    if (S == 1) {
        if (result == 0) {
            machine->registers[CPSR] |= zMask;
        }
        machine->registers[CPSR] |= (result >> (WORD_SIZE - 1));
    }  
}
