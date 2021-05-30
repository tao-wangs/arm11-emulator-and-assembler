#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "decode.h"
#include "singledatatransfer.h"

void decodeSDT(unsigned int instruction, ARM_STATE *machinePtr) {

    int Cond = (instruction & COND_MASK) >> 28;
    int I = (instruction & I_MASK) >> 25;
    int P = (instruction & P_MASK) >> 24;
    int U = (instruction & U_MASK) >> 23;
    int L = (instruction & L_MASK) >> 20;
    int Rn = (instruction & RN_MASK) >> 16;
    int Rd = (instruction & RD_MASK) >> 12;
    int Offset = (instruction & OFFSET_MASK);

    int shift = (Offset & SHIFT_MASK) >> 4;
    int Rm = (Offset & RM_MASK);

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
                        rotateRightSDT((unsigned int) machinePtr->registers[Rm], integer);
            }
        } else { //this is the optional part on page 7 of spec
            assert (bit4 == 1);

            int shiftAmount = (machinePtr->registers[Rs] & 0x000000FF); //last byte of Rs

            switch (shiftType) {
                case LSL:
                    Offset = machinePtr->registers[Rm] =
                        (unsigned int) machinePtr->registers[Rm] << shiftAmount;
                    break;
                case LSR:
                    Offset = machinePtr->registers[Rm] =
                        (unsigned int) machinePtr->registers[Rm] >> shiftAmount;
                    break;
                case ASR:
                    Offset = machinePtr->registers[Rm] =
                        machinePtr->registers[Rm] >> shiftAmount;
                    break;
                default:
                    assert (shiftType == ROR);
                    Offset = machinePtr->registers[Rm] =
                        rotateRightSDT((unsigned int) machinePtr->registers[Rm], shiftAmount);
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


unsigned int rotateRightSDT(unsigned int value, int shift) {
    if ((shift &= 31) == 0) {
      return value;
    }
    return (value >> shift) | (value << (32 - shift));
}
