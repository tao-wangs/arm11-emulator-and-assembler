#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "arm_state.h"

typedef enum {
    EQ = 0x0,
    NE = 0x1,
    GE = 0xA, 
    LT = 0xB,
    GT = 0xC, 
    LE = 0xD, 
    AL = 0xE
} CONDITION_CODE;

void decodeMultiply(unsigned int instruction, ARM_STATE *machinePtr);
bool conditionMet(unsigned int conditionCode, ARM_STATE *machine);
void executeMultiply(int Rm, int Rs, int Rd, int S, ARM_STATE *machine);
void executeMultiplyAccumulate(int Rm, int Rs, int Rn, int Rd, int S, ARM_STATE *machine);

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

/* flags in CPSR:
n = last result was negative
z = last result was zero
c = last result caused a bit to be carried out
v = last result overflowed */
bool conditionMet(unsigned int conditionCode, ARM_STATE *machine) {
    int nMask = 0x8000000;
    int zMask = 0x4000000;
    int cMask = 0x2000000;
    int vMask = 0x1000000;

    unsigned char n = (machine->registers[CPSR] & nMask) >> 31; 
    unsigned char z = (machine->registers[CPSR] & zMask) >> 30; 
    unsigned char c = (machine->registers[CPSR] & cMask) >> 29; 
    unsigned char v = (machine->registers[CPSR] & vMask) >> 28; 

    switch (conditionCode) {
        case EQ:
            return z;
        case NE: 
            return !z;
        case GE : 
            return (n == v);
        case LT: 
            return !(n == v);
        case GT: 
            return (!z && (n == v));
        case LE: 
            return (z || !(n == v));
        case AL:  
            return true;
        default: 
            return false;
    }
}
