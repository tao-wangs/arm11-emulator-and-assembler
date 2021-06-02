#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>

#include "decode.h"
#include "singledatatransfer.h"

void decodeSDT(uint32_t instruction, ARM_STATE *machinePtr) {

    int32_t Cond = (instruction & COND_MASK) >> 28;
    int32_t I = (instruction & I_MASK) >> 25;
    int32_t P = (instruction & P_MASK) >> 24;
    int32_t U = (instruction & U_MASK) >> 23;
    int32_t L = (instruction & L_MASK) >> 20;
    int32_t Rn = (instruction & RN_MASK) >> 16;
    int32_t Rd = (instruction & RD_MASK) >> 12;
    int32_t Offset = (instruction & OFFSET_MASK);

    int32_t shift = (Offset & SHIFT_MASK) >> 4;
    int32_t Rm = (Offset & RM_MASK);

    int32_t bit4 = (shift & 0x01);

    int32_t integer = (shift & 0xF8) >> 3;
    int32_t Rs = (shift & 0xF0) >> 4;

    int32_t shiftType = (shift & 0x06) >> 1;

    if (!conditionMet(Cond, machinePtr)) {
        printf("Condition not met. SDT instruction skipped");
        return;
    }

    //this section can definitely be condensed, not sure how yet though.
    if (I == 1) { //means offset is a shifted register
        if (bit4 == 0) {
            switch (shiftType) {
                case LSL: ;
                    Offset =
                        machinePtr->registers[Rm] << integer;
                    break;
                case LSR: ;
                    Offset =
                        (uint32_t) machinePtr->registers[Rm] >> integer;
                    break;
                case ASR: ;
                    Offset =
                        machinePtr->registers[Rm] >> integer;
                    break;
                default: ;
                    assert (shiftType == ROR);
                    Offset =
                        rotateRightSDT(machinePtr->registers[Rm], integer);
            }
        } else { //this is the optional part on page 7 of spec
            assert (bit4 == 1);

            int32_t shiftAmount = (machinePtr->registers[Rs] & 0x000000FF); //last byte of Rs

            switch (shiftType) {
                case LSL: ;
                    Offset =
                        machinePtr->registers[Rm] << shiftAmount;
                    break;
                case LSR: ;
                    Offset =
                        (uint32_t) machinePtr->registers[Rm] >> shiftAmount;
                    break;
                case ASR: ;
                    Offset =
                        machinePtr->registers[Rm] >> shiftAmount;
                    break;
                default: ;
                    assert (shiftType == ROR);
                    Offset =
                        rotateRightSDT(machinePtr->registers[Rm], shiftAmount);
            }
        }
        (L == 1) ?
        executeLoad(P, U, Rn, Rd, Offset, machinePtr) :
        executeStore(P, U, Rn, Rd, Offset, machinePtr);
    } else {
        (L == 1) ?
        executeLoad(P, U, Rn, Rd, Offset, machinePtr) :
        executeStore(P, U, Rn, Rd, Offset, machinePtr);
    }
}

void executeLoad(int32_t P, int32_t U, int32_t Rn, int32_t Rd, uint32_t Offset, ARM_STATE *machine) { //Rn is the base register

    int32_t baseAddress = machine->registers[Rn];
    int32_t address = (U == 1) ? baseAddress + Offset : baseAddress - Offset;

    if (address > NUM_ALLIGNED_ADDRS) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
        return;
    }

    if (P == 1) { //pre indexing
        switch(address % WORD_SIZE) {
            case 0: ;
                machine->registers[Rd] = toLittleEndian(machine->memory[address / WORD_SIZE]);
                break;
            case 1: ;
                int32_t first3Bytes = machine->memory[address / WORD_SIZE] & 0x00FFFFFF;
                int32_t lastByte = ((uint32_t) machine->memory[(address / WORD_SIZE) + 1] & 0xFF000000) >> 24;
                machine->registers[Rd] = toLittleEndian((first3Bytes << 8) | lastByte);
                break;
            case 2: ;
                int32_t first2Bytes = machine->memory[address / WORD_SIZE] & 0x0000FFFF;
                int32_t last2Bytes = ((uint32_t) machine->memory[(address / WORD_SIZE) + 1] & 0xFFFF0000) >> 16;
                machine->registers[Rd] = toLittleEndian((first2Bytes << 16) | last2Bytes);
                break;
            case 3: ;
                int32_t firstByte = machine->memory[address / WORD_SIZE] & 0x000000FF;
                int32_t last3Bytes = ((uint32_t) machine->memory[(address / WORD_SIZE) + 1] & 0xFFFFFF00) >> 8;
                machine->registers[Rd] = toLittleEndian((firstByte << 24) | last3Bytes);
                break;
        }
        return; //exit method
    } //post indexing
    switch(baseAddress % WORD_SIZE) {
            case 0: ;
                machine->registers[Rd] = toLittleEndian(machine->memory[baseAddress / 4]);
                break;
            case 1: ;
                int32_t first3Bytes = machine->memory[baseAddress / WORD_SIZE] & 0x00FFFFFF;
                int32_t lastByte = ((uint32_t) machine->memory[(baseAddress / WORD_SIZE) + 1] & 0xFF000000) >> 24;
                machine->registers[Rd] = toLittleEndian((first3Bytes << 8) | lastByte);
                break;
            case 2: ;
                int32_t first2Bytes = machine->memory[baseAddress / WORD_SIZE] & 0x0000FFFF;
                int32_t last2Bytes = ((uint32_t) machine->memory[(baseAddress / WORD_SIZE) + 1] & 0xFFFF0000) >> 16;
                machine->registers[Rd] = toLittleEndian((first2Bytes << 16) | last2Bytes);
                break;
            case 3: ;
                int32_t firstByte = machine->memory[baseAddress / WORD_SIZE] & 0x000000FF;
                int32_t last3Bytes = ((uint32_t) machine->memory[(baseAddress / WORD_SIZE) + 1] & 0xFFFFFF00) >> 8;
                machine->registers[Rd] = toLittleEndian((firstByte << 24) | last3Bytes);
                break;
        }
    machine->registers[Rn] = address;
}

void executeStore(int32_t P, int32_t U, int32_t Rn, int32_t Rd, int32_t Offset, ARM_STATE *machine) { //this code is literally unreadable but it works ;)
    int32_t baseAddress = machine->registers[Rn];
    int32_t address = (U == 1) ? baseAddress + Offset : baseAddress - Offset;

    if (address > NUM_ALLIGNED_ADDRS) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
        return;
    }

    if (P == 1) {
        switch(address % WORD_SIZE) {
            case 0: ;
                machine->memory[address / WORD_SIZE] = toLittleEndian(machine->registers[Rd]);
                break;
            case 1: ;
                int32_t newReplacement3Bytes2 = toLittleEndian(machine->registers[Rd]) & 0xFFFFFF00;
                int32_t newReplacement1Byte2 = toLittleEndian(machine->registers[Rd]) & 0x000000FF;

                int32_t oldByte2 = machine->memory[address / WORD_SIZE] & 0xFF000000;
                int32_t old3Bytes2 = machine->memory[(address / WORD_SIZE) + 1] & 0x00FFFFFF;

                machine->memory[address / WORD_SIZE] = oldByte2 | ((uint32_t) newReplacement3Bytes2 >> 8);
                machine->memory[(address / WORD_SIZE) + 1] = old3Bytes2 | (newReplacement1Byte2 << 24);
                break;
            case 2: ;
                int32_t newReplacementFirst2Bytes = toLittleEndian(machine->registers[Rd]) & 0xFFFF0000;
                int32_t newReplacementLast2Bytes = toLittleEndian(machine->registers[Rd]) & 0x0000FFFF;

                int32_t oldFirst2Bytes = machine->memory[address / WORD_SIZE] & 0xFFFF0000;
                int32_t oldLast2Bytes = machine->memory[(address / WORD_SIZE) + 1] & 0x0000FFFF;

                machine->memory[address / WORD_SIZE] = oldFirst2Bytes | ((uint32_t) newReplacementFirst2Bytes >> 16);
                machine->memory[(address / WORD_SIZE) + 1] = oldLast2Bytes | (newReplacementLast2Bytes << 16);
                break;
            case 3: ;
                int32_t newReplacement1Byte3 = toLittleEndian(machine->registers[Rd]) & 0xFF000000;
                int32_t newReplacement3Bytes3 = toLittleEndian(machine->registers[Rd]) & 0x00FFFFFF;

                int32_t old3Bytes3 = machine->memory[address / WORD_SIZE] & 0xFFFFFF00;
                int32_t oldByte3 = machine->memory[(address / WORD_SIZE) + 1] & 0x000000FF;

                machine->memory[address / WORD_SIZE] = old3Bytes3 | ((uint32_t) newReplacement1Byte3 >> 24);
                machine->memory[(address / WORD_SIZE) + 1] = oldByte3 | (newReplacement3Bytes3 << 8);
                break;
        }
        return; //exit method
    }

    switch(baseAddress % WORD_SIZE) {
            case 0: ;
                machine->memory[baseAddress / WORD_SIZE] = toLittleEndian(machine->registers[Rd]);
                break;
            case 1: ;
                int32_t newReplacement3Bytes1 = toLittleEndian(machine->registers[Rd]) & 0xFFFFFF00;
                int32_t newReplacement1Byte1 = toLittleEndian(machine->registers[Rd]) & 0x000000FF;

                int32_t oldByte1 = machine->memory[baseAddress / WORD_SIZE] & 0xFF000000;
                int32_t old3Bytes1 = machine->memory[(baseAddress / WORD_SIZE) + 1] & 0x00FFFFFF;

                machine->memory[baseAddress / WORD_SIZE] = oldByte1 | ((uint32_t) newReplacement3Bytes1 >> 8);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = old3Bytes1 | (newReplacement1Byte1 << 24); 
                break;
            case 2: ;
                int32_t newReplacementFirst2Bytes = toLittleEndian(machine->registers[Rd]) & 0xFFFF0000;
                int32_t newReplacementLast2Bytes = toLittleEndian(machine->registers[Rd]) & 0x0000FFFF;

                int32_t oldFirst2Bytes = machine->memory[baseAddress / WORD_SIZE] & 0xFFFF0000;
                int32_t oldLast2Bytes = machine->memory[(baseAddress / WORD_SIZE) + 1] & 0x0000FFFF;

                machine->memory[baseAddress / WORD_SIZE] = oldFirst2Bytes | ((uint32_t) newReplacementFirst2Bytes >> 16);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = oldLast2Bytes | (newReplacementLast2Bytes << 16); 
                break;
            case 3: ;
                int32_t newReplacement1Byte = toLittleEndian(machine->registers[Rd]) & 0xFF000000;
                int32_t newReplacement3Bytes = toLittleEndian(machine->registers[Rd]) & 0x00FFFFFF;

                int32_t old3Bytes = machine->memory[baseAddress / WORD_SIZE] & 0xFFFFFF00;
                int32_t oldByte = machine->memory[(baseAddress / WORD_SIZE) + 1] & 0x000000FF;

                machine->memory[baseAddress / WORD_SIZE] = old3Bytes | ((uint32_t) newReplacement1Byte >> 24);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = oldByte | (newReplacement3Bytes << 8);
                break;
        }
    machine->registers[Rn] = address;
}


uint32_t rotateRightSDT(uint32_t value, int32_t shift) {
    if ((shift &= 31) == 0) {
      return value;
    }
    return (value >> shift) | (value << (32 - shift));
}
