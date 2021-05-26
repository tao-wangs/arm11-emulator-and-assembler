#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
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

typedef enum {
    LSL,
    LSR,
    ASR,
    ROR,
} SHIFT_TYPE;

void decodeSDT(unsigned int instruction, ARM_STATE *machinePtr);
void executeLoad(int P, int U, int Rn, int Rd, int Offset, ARM_STATE *machine);
void executeStore(int P, int U, int Rn, int Rd, int Offset, ARM_STATE *machine);
unsigned int rotateRight(unsigned int value, int shift);
bool conditionMet(unsigned int conditionCode, ARM_STATE *machine);

void decodeSDT(unsigned int instruction, ARM_STATE *machinePtr) {
    int condMask = 0xF0000000;
    int iMask = 0x02000000;
    int pMask = 0x01000000;
    int uMask = 0x00800000;
    int lMask = 0x00100000;
    int rnMask = 0x000F0000;
    int rdMask = 0x0000F000;
    int offsetMask = 0x00000FFF;

    int Cond = (instruction & condMask) >> 28;
    int I = (instruction & iMask) >> 25;
    int P = (instruction & pMask) >> 24;
    int U = (instruction & uMask) >> 23;
    int L = (instruction & lMask) >> 20;
    int Rn = (instruction & rnMask) >> 16;
    int Rd = (instruction & rdMask) >> 12;
    int Offset = (instruction & offsetMask);

    int shiftMask = 0x00000FF0;
    int rmMask = 0x0000000F;

    int shift = (Offset & shiftMask) >> 4;
    int Rm = (Offset & rmMask);

    int bit4 = (shift & 0x01);
    
    int integer = (shift & 0xF8) >> 3; 
    int Rs = (shift & 0xF0) >> 4;
    
    int shiftType = (shift & 0x06) >> 1;

    if (!conditionMet(Cond, machinePtr)) {
        return;
    }    


    //this section can definitely be condensed, not sure how yet though.
    if (I == 1) { //then offset is a shifted register
        if (bit4 == 0) {
            switch (shiftType) {
                case LSL: ;
                    Offset = machinePtr->registers[Rm] = 
                        (unsigned int) machinePtr->registers[Rm] << integer;
                    break;
                case LSR: ;
                    Offset = machinePtr->registers[Rm] =
                        (unsigned int) machinePtr->registers[Rm] >> integer;
                    break;
                case ASR: ;
                    Offset = machinePtr->registers[Rm] = 
                        machinePtr->registers[Rm] >> integer;
                    break;
                default: ;
                    assert (shiftType == ROR);
                    Offset = machinePtr->registers[Rm] = 
                        rotateRight((unsigned int) machinePtr->registers[Rm], integer);
            }
        } else { //this is the optional part on page 7 of spec
            assert (bit4 == 1);
            
            int shiftAmount = (machinePtr->registers[Rs] & 0x000000FF); //last byte of Rs

            switch (shiftType) {
                case LSL: ;
                    Offset = machinePtr->registers[Rm] = 
                        (unsigned int) machinePtr->registers[Rm] << shiftAmount;
                    break;
                case LSR: ;
                    Offset = machinePtr->registers[Rm] =
                        (unsigned int) machinePtr->registers[Rm] >> shiftAmount;
                    break;
                case ASR: ;
                    Offset = machinePtr->registers[Rm] = 
                        machinePtr->registers[Rm] >> shiftAmount;
                    break;
                default: ;
                    assert (shiftType == ROR);
                    Offset = machinePtr->registers[Rm] = 
                        rotateRight((unsigned int) machinePtr->registers[Rm], shiftAmount);
            }
        }
        (L == 1) ? executeLoad(P, U, Rn, Rd, Offset, machinePtr) : executeStore(P, U, Rn, Rd, Offset, machinePtr);
    } else {
        (L == 1) ? 
        executeLoad(P, U, Rn, Rd, (unsigned int) Offset, machinePtr) : 
        executeStore(P, U, Rn, Rd, (unsigned int) Offset, machinePtr);
    }
}

void executeLoad(int P, int U, int Rn, int Rd, int Offset, ARM_STATE *machine) { //Rn is the base register
    int baseValue = machine->registers[Rn];
    int index = (U == 1) ? baseValue + Offset * WORD_SIZE : baseValue - Offset * WORD_SIZE;
    if (P == 1) { //pre indexing
        machine->registers[Rd] = machine->memory[index / WORD_SIZE];
        return; //exit method
    } //post indexing
    machine->registers[Rd] = machine->memory[index / WORD_SIZE];
    machine->registers[Rn] = index;
}

void executeStore(int P, int U, int Rn, int Rd, int Offset, ARM_STATE *machine) {
    int baseValue = machine->registers[Rn];
    int index = (U == 1) ? baseValue + Offset * WORD_SIZE : baseValue - Offset * WORD_SIZE;
    if (P == 1) {
        machine->memory[index / WORD_SIZE] = machine->registers[Rd];
        return; //exit method
    }
    machine->memory[baseValue] = machine->registers[Rd];
    machine->registers[Rn] = index;
}


unsigned int rotateRight(unsigned int value, int shift) {
    if ((shift &= 31) == 0) {
      return value;
    }
    return (value >> shift) | (value << (32 - shift));
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