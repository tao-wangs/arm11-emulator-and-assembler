#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "decode.h"
#include "singledatatransfer.h"

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
                        (unsigned int) machinePtr->registers[Rm] >> integer;
                    break;
                case ASR: ;
                    Offset = 
                        machinePtr->registers[Rm] >> integer;
                    break;
                default: ;
                    assert (shiftType == ROR);
                    Offset =  
                        rotateRightSDT((unsigned int) machinePtr->registers[Rm], integer);
            }
        } else { //this is the optional part on page 7 of spec
            assert (bit4 == 1);
            
            int shiftAmount = (machinePtr->registers[Rs] & 0x000000FF); //last byte of Rs

            switch (shiftType) {
                case LSL: ;
                    Offset = 
                        machinePtr->registers[Rm] << shiftAmount;
                    break;
                case LSR: ;
                    Offset = 
                        (unsigned int) machinePtr->registers[Rm] >> shiftAmount;
                    break;
                case ASR: ;
                    Offset = 
                        machinePtr->registers[Rm] >> shiftAmount;
                    break;
                default: ;
                    assert (shiftType == ROR);
                    Offset =  
                        rotateRightSDT((unsigned int) machinePtr->registers[Rm], shiftAmount);
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

void executeLoad(int P, int U, int Rn, int Rd, unsigned int Offset, ARM_STATE *machine) { //Rn is the base register

    int baseAddress = machine->registers[Rn];
    int address = (U == 1) ? baseAddress + Offset : baseAddress - Offset;

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
                int first3Bytes = machine->memory[address / WORD_SIZE] & 0x00FFFFFF;
                int lastByte = ((unsigned int) machine->memory[(address / WORD_SIZE) + 1] & 0xFF000000) >> 24;
                machine->registers[Rd] = toLittleEndian((first3Bytes << 8) | lastByte);
                break;
            case 2: ;
                int first2Bytes = machine->memory[address / WORD_SIZE] & 0x0000FFFF; 
                int last2Bytes = ((unsigned int) machine->memory[(address / WORD_SIZE) + 1] & 0xFFFF0000) >> 16;
                machine->registers[Rd] = toLittleEndian((first2Bytes << 16) | last2Bytes);
                break;
            case 3: ;
                int firstByte = machine->memory[address / WORD_SIZE] & 0x000000FF;
                int last3Bytes = ((unsigned int) machine->memory[(address / WORD_SIZE) + 1] & 0xFFFFFF00) >> 8;
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
                int first3Bytes = machine->memory[address / WORD_SIZE] & 0x00FFFFFF;
                int lastByte = ((unsigned int) machine->memory[(baseAddress / WORD_SIZE) + 1] & 0xFF000000) >> 24;
                machine->registers[Rd] = toLittleEndian((first3Bytes << 8) | lastByte);
                break;
            case 2: ;
                int first2Bytes = machine->memory[address / WORD_SIZE] & 0x0000FFFF; 
                int last2Bytes = ((unsigned int) machine->memory[(baseAddress / WORD_SIZE) + 1] & 0xFFFF0000) >> 16;
                machine->registers[Rd] = toLittleEndian((first2Bytes << 16) | last2Bytes);
                break;
            case 3: ;
                int firstByte = machine->memory[address / WORD_SIZE] & 0x000000FF;
                int last3Bytes = ((unsigned int) machine->memory[(baseAddress / WORD_SIZE) + 1] & 0xFFFFFF00) >> 8;
                machine->registers[Rd] = toLittleEndian((firstByte << 24) | last3Bytes);
                break;
        }
    machine->registers[Rn] = address;
}

void executeStore(int P, int U, int Rn, int Rd, int Offset, ARM_STATE *machine) { //this code is literally unreadable but it works ;)
    int baseAddress = machine->registers[Rn];
    int address = (U == 1) ? baseAddress + Offset : baseAddress - Offset;

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
                int newReplacement3Bytes2 = toLittleEndian(machine->registers[Rd]) & 0xFFFFFF00;
                int newReplacement1Byte2 = toLittleEndian(machine->registers[Rd]) & 0x000000FF;

                int oldByte2 = machine->memory[address / WORD_SIZE] & 0xFF000000;
                int old3Bytes2 = machine->memory[(address / WORD_SIZE) + 1] & 0x00FFFFFF;
                
                machine->memory[address / WORD_SIZE] = oldByte2 | ((unsigned int) newReplacement3Bytes2 >> 8);
                machine->memory[(address / WORD_SIZE) + 1] = old3Bytes2 | (newReplacement1Byte2 << 24); 
                break;
            case 2: ;
                int newReplacementFirst2Bytes = toLittleEndian(machine->registers[Rd]) & 0xFFFF0000;
                int newReplacementLast2Bytes = toLittleEndian(machine->registers[Rd]) & 0x0000FFFF;

                int oldFirst2Bytes = machine->memory[address / WORD_SIZE] & 0xFFFF0000;
                int oldLast2Bytes = machine->memory[(address / WORD_SIZE) + 1] & 0x0000FFFF;
                
                machine->memory[address / WORD_SIZE] = oldFirst2Bytes | ((unsigned int) newReplacementFirst2Bytes >> 16);
                machine->memory[(address / WORD_SIZE) + 1] = oldLast2Bytes | (newReplacementLast2Bytes << 16); 
                break;
            case 3: ;
                int newReplacement1Byte3 = toLittleEndian(machine->registers[Rd]) & 0xFF000000;
                int newReplacement3Bytes3 = toLittleEndian(machine->registers[Rd]) & 0x00FFFFFF;

                int old3Bytes3 = machine->memory[address / WORD_SIZE] & 0xFFFFFF00;
                int oldByte3 = machine->memory[(address / WORD_SIZE) + 1] & 0x000000FF;

                machine->memory[address / WORD_SIZE] = old3Bytes3 | ((unsigned int) newReplacement1Byte3 >> 24);
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
                int newReplacement3Bytes1 = toLittleEndian(machine->registers[Rd]) & 0xFFFFFF00;
                int newReplacement1Byte1 = toLittleEndian(machine->registers[Rd]) & 0x000000FF;

                int oldByte1 = machine->memory[baseAddress / WORD_SIZE] & 0xFF000000;
                int old3Bytes1 = machine->memory[(baseAddress / WORD_SIZE) + 1] & 0x00FFFFFF;
                
                machine->memory[baseAddress / WORD_SIZE] = oldByte1 | ((unsigned int) newReplacement3Bytes1 >> 8);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = old3Bytes1 | (newReplacement1Byte1 << 24); 
                break;
            case 2: ;
                int newReplacementFirst2Bytes = toLittleEndian(machine->registers[Rd]) & 0xFFFF0000;
                int newReplacementLast2Bytes = toLittleEndian(machine->registers[Rd]) & 0x0000FFFF;

                int oldFirst2Bytes = machine->memory[baseAddress / WORD_SIZE] & 0xFFFF0000;
                int oldLast2Bytes = machine->memory[(baseAddress / WORD_SIZE) + 1] & 0x0000FFFF;
                
                machine->memory[baseAddress / WORD_SIZE] = oldFirst2Bytes | ((unsigned int) newReplacementFirst2Bytes >> 16);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = oldLast2Bytes | (newReplacementLast2Bytes << 16); 
                break;
            case 3: ;
                int newReplacement1Byte = toLittleEndian(machine->registers[Rd]) & 0xFF000000;
                int newReplacement3Bytes = toLittleEndian(machine->registers[Rd]) & 0x00FFFFFF;

                int old3Bytes = machine->memory[baseAddress / WORD_SIZE] & 0xFFFFFF00;
                int oldByte = machine->memory[(baseAddress / WORD_SIZE) + 1] & 0x000000FF;

                machine->memory[baseAddress / WORD_SIZE] = old3Bytes | ((unsigned int) newReplacement1Byte >> 24);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = oldByte | (newReplacement3Bytes << 8);
                break;
        }
    machine->registers[Rn] = address;
}


unsigned int rotateRightSDT(unsigned int value, int shift) {
    if ((shift &= 31) == 0) {
      return value;
    }
    return (value >> shift) | (value << (32 - shift));
}