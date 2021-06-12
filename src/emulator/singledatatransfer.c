
#include "singledatatransfer.h"

void decodeSDT(uint32_t instruction, ARM_STATE *machinePtr) {

    int32_t condCode = (instruction >> CONDCODE_SHIFT) & FOUR_BIT_MASK;
    int32_t immOffset = (instruction >> IMM_OPERAND_SHIFT) & ONE_BIT_MASK;
    int32_t pIndexingBit = (instruction >> P_SHIFT) & ONE_BIT_MASK;
    int32_t upBit = (instruction >> U_SHIFT) & ONE_BIT_MASK;
    int32_t loadBit = (instruction >> L_SHIFT) & ONE_BIT_MASK;
    int32_t rn = (instruction >> RN_SHIFT_SDT) & FOUR_BIT_MASK;
    int32_t rd = (instruction >> RD_SHIFT_SDT) & FOUR_BIT_MASK;
    int32_t offset = (instruction & OFFSET_MASK_SDT);

    int32_t shift = (offset >> SHIFT_SHIFT) & EIGHT_BIT_MASK;
    int32_t rm = (offset & FOUR_BIT_MASK);

    int32_t bit4 = (shift & ONE_BIT_MASK);

    int32_t integer = (shift >> INTEGER_SHIFT) & FIVE_BIT_MASK;
    int32_t rs = (shift >> RS_SHIFT_SDT) & FOUR_BIT_MASK;

    int32_t shiftType = (shift >> TYPE_SHIFT) & TWO_BIT_MASK;

    if (!conditionMet(condCode, machinePtr)) {
        printf("Condition not met. SDT instruction skipped");
        return;
    }

    //this section can definitely be condensed, not sure how yet though.
    if (immOffset == 1) { //means offset is a shifted register
        if (bit4 == 0) {
            switch (shiftType) {
                case LSL:
                    offset = machinePtr->registers[rm] << integer;
                    break;
                case LSR: 
                    offset = (uint32_t) machinePtr->registers[rm] >> integer;
                    break;
                case ASR: 
                    offset = machinePtr->registers[rm] >> integer;
                    break;
                default: 
                    assert (shiftType == ROR);
                    offset = rotateRight(machinePtr->registers[rm], integer);
            }
        } else { //this is the optional part on page 7 of spec
            assert (bit4 == 1);

            int32_t shiftAmount = (machinePtr->registers[rs] & EIGHT_BIT_MASK); //last byte of Rs

            switch (shiftType) {
                case LSL:
                    offset = machinePtr->registers[rm] << shiftAmount;
                    break;
                case LSR:
                    offset = (uint32_t) machinePtr->registers[rm] >> shiftAmount;
                    break;
                case ASR:
                    offset = machinePtr->registers[rm] >> shiftAmount;
                    break;
                default:
                    assert (shiftType == ROR);
                    offset = rotateRight(machinePtr->registers[rm], shiftAmount);
            }
        }
    }
    
    (loadBit == 1) ?
    executeLoad(pIndexingBit, upBit, rn, rd, offset, machinePtr) :
    executeStore(pIndexingBit, upBit, rn, rd, offset, machinePtr);
}

void executeLoad(int32_t pIndexingBit, int32_t upBit, int32_t rn, int32_t rd, uint32_t offset, ARM_STATE *machine) { //Rn is the base register

    int32_t baseAddress = machine->registers[rn];
    int32_t address = (upBit == 1) ? baseAddress + offset : baseAddress - offset;

    if (address == 0x20200000) {
        printf("One GPIO pin from 0 to 9 has been accessed\n");
        machine->registers[rd] = address;
        return;
    } else if (address == 0x20200004) {
        printf("One GPIO pin from 10 to 19 has been accessed\n");
        machine->registers[rd] = address;
        return;
    } else if (address == 0x20200008) {
        printf("One GPIO pin from 20 to 29 has been accessed\n");
        machine->registers[rd] = address;
        return;
    }

    if (address > NUM_ALLIGNED_ADDRS) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
        return;
    }

    if (pIndexingBit == 1) { //pre indexing
        switch(address % WORD_SIZE) {
            case 0: ;
                machine->registers[rd] = toLittleEndian(machine->memory[address / WORD_SIZE]);
                break;
            case 1: ;
                int32_t first3Bytes = machine->memory[address / WORD_SIZE] & THREE_BYTE_MASK;
                int32_t lastByte = ((uint32_t) machine->memory[(address / WORD_SIZE) + 1] >> THREE_BYTE_SHIFT) & EIGHT_BIT_MASK;
                machine->registers[rd] = toLittleEndian((first3Bytes << ONE_BYTE_SHIFT) | lastByte);
                break;
            case 2: ;
                int32_t first2Bytes = machine->memory[address / WORD_SIZE] & TWO_BYTE_MASK;
                int32_t last2Bytes = ((uint32_t) machine->memory[(address / WORD_SIZE) + 1] >> TWO_BYTE_SHIFT) & TWO_BYTE_MASK;
                machine->registers[rd] = toLittleEndian((first2Bytes << TWO_BYTE_SHIFT) | last2Bytes);
                break;
            case 3: ;
                int32_t firstByte = machine->memory[address / WORD_SIZE] & EIGHT_BIT_MASK;
                int32_t last3Bytes = ((uint32_t) machine->memory[(address / WORD_SIZE) + 1] >> ONE_BYTE_SHIFT) & THREE_BYTE_MASK;
                machine->registers[rd] = toLittleEndian((firstByte << THREE_BYTE_SHIFT) | last3Bytes);
                break;
        }
        return; //exit method
    } //post indexing
    switch(baseAddress % WORD_SIZE) {
            case 0: ;
                machine->registers[rd] = toLittleEndian(machine->memory[baseAddress / 4]);
                break;
            case 1: ;
                int32_t first3Bytes = machine->memory[baseAddress / WORD_SIZE] & THREE_BYTE_MASK;
                int32_t lastByte = ((uint32_t) machine->memory[(baseAddress / WORD_SIZE) + 1] >> THREE_BYTE_SHIFT) & EIGHT_BIT_MASK;
                machine->registers[rd] = toLittleEndian((first3Bytes << ONE_BYTE_SHIFT) | lastByte);
                break;
            case 2: ;
                int32_t first2Bytes = machine->memory[baseAddress / WORD_SIZE] & TWO_BYTE_MASK;
                int32_t last2Bytes = ((uint32_t) machine->memory[(baseAddress / WORD_SIZE) + 1] >> TWO_BYTE_SHIFT) & TWO_BYTE_MASK;
                machine->registers[rd] = toLittleEndian((first2Bytes << TWO_BYTE_SHIFT) | last2Bytes);
                break;
            case 3: ;
                int32_t firstByte = machine->memory[baseAddress / WORD_SIZE] & EIGHT_BIT_MASK;
                int32_t last3Bytes = ((uint32_t) machine->memory[(baseAddress / WORD_SIZE) + 1] >> ONE_BYTE_SHIFT) & THREE_BYTE_MASK;
                machine->registers[rd] = toLittleEndian((firstByte << THREE_BYTE_SHIFT) | last3Bytes);
                break;
        }
    machine->registers[rn] = address;
}

// since this is similar to previous, we could try a way to combine the two?
void executeStore(int32_t pIndexingBit, int32_t upBit, int32_t rn, int32_t rd, int32_t offset, ARM_STATE *machine) {
    int32_t baseAddress = machine->registers[rn];
    int32_t address = (upBit == 1) ? baseAddress + offset : baseAddress - offset;

    if (address == 0x20200000) {
        printf("One GPIO pin from 0 to 9 has been accessed\n");
        machine->registers[rd] = address;
        return;
    } else if (address == 0x20200004) {
        printf("One GPIO pin from 10 to 19 has been accessed\n");
        machine->registers[rd] = address;
        return;
    } else if (address == 0x20200008) {
        printf("One GPIO pin from 20 to 29 has been accessed\n");
        machine->registers[rd] = address;
        return;
    } else if (address == 0x20200028) {
        printf("PIN OFF\n");
        return;
    } else if (address == 0x2020001C) {
        printf("PIN ON\n");
        return;
    }

    if (address > NUM_ALLIGNED_ADDRS) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
        return;
    }

    if (pIndexingBit == 1) {
        switch(address % WORD_SIZE) {
            case 0: ;
                machine->memory[address / WORD_SIZE] = toLittleEndian(machine->registers[rd]);
                break;
            case 1: ;
                int32_t newReplacement3Bytes2 = toLittleEndian(machine->registers[rd]) & 0xFFFFFF00;
                int32_t newReplacement1Byte2 = toLittleEndian(machine->registers[rd]) & EIGHT_BIT_MASK;

                int32_t oldByte2 = machine->memory[address / WORD_SIZE] & 0xFF000000;
                int32_t old3Bytes2 = machine->memory[(address / WORD_SIZE) + 1] & THREE_BYTE_MASK;

                machine->memory[address / WORD_SIZE] = oldByte2 | ((uint32_t) newReplacement3Bytes2 >> ONE_BYTE_SHIFT);
                machine->memory[(address / WORD_SIZE) + 1] = old3Bytes2 | (newReplacement1Byte2 << THREE_BYTE_SHIFT);
                break;
            case 2: ;
                int32_t newReplacementFirst2Bytes = toLittleEndian(machine->registers[rd]) & 0xFFFF0000;
                int32_t newReplacementLast2Bytes = toLittleEndian(machine->registers[rd]) & TWO_BYTE_MASK;

                int32_t oldFirst2Bytes = machine->memory[address / WORD_SIZE] & 0xFFFF0000;
                int32_t oldLast2Bytes = machine->memory[(address / WORD_SIZE) + 1] & TWO_BYTE_MASK;

                machine->memory[address / WORD_SIZE] = oldFirst2Bytes | ((uint32_t) newReplacementFirst2Bytes >> TWO_BYTE_SHIFT);
                machine->memory[(address / WORD_SIZE) + 1] = oldLast2Bytes | (newReplacementLast2Bytes << TWO_BYTE_SHIFT);
                break;
            case 3: ;
                int32_t newReplacement1Byte3 = toLittleEndian(machine->registers[rd]) & 0xFF000000;
                int32_t newReplacement3Bytes3 = toLittleEndian(machine->registers[rd]) & THREE_BYTE_MASK;

                int32_t old3Bytes3 = machine->memory[address / WORD_SIZE] & 0xFFFFFF00;
                int32_t oldByte3 = machine->memory[(address / WORD_SIZE) + 1] & EIGHT_BIT_MASK;

                machine->memory[address / WORD_SIZE] = old3Bytes3 | ((uint32_t) newReplacement1Byte3 >> THREE_BYTE_SHIFT);
                machine->memory[(address / WORD_SIZE) + 1] = oldByte3 | (newReplacement3Bytes3 << ONE_BYTE_SHIFT);
                break;
        }
        return; //exit method
    }

    switch(baseAddress % WORD_SIZE) {
            case 0: ;
                machine->memory[baseAddress / WORD_SIZE] = toLittleEndian(machine->registers[rd]);
                break;
            case 1: ;
                int32_t newReplacement3Bytes1 = toLittleEndian(machine->registers[rd]) & 0xFFFFFF00;
                int32_t newReplacement1Byte1 = toLittleEndian(machine->registers[rd]) & EIGHT_BIT_MASK;

                int32_t oldByte1 = machine->memory[baseAddress / WORD_SIZE] & 0xFF000000;
                int32_t old3Bytes1 = machine->memory[(baseAddress / WORD_SIZE) + 1] & THREE_BYTE_MASK;

                machine->memory[baseAddress / WORD_SIZE] = oldByte1 | ((uint32_t) newReplacement3Bytes1 >> ONE_BYTE_SHIFT);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = old3Bytes1 | (newReplacement1Byte1 << THREE_BYTE_SHIFT); 
                break;
            case 2: ;
                int32_t newReplacementFirst2Bytes = toLittleEndian(machine->registers[rd]) & 0xFFFF0000;
                int32_t newReplacementLast2Bytes = toLittleEndian(machine->registers[rd]) & TWO_BYTE_MASK;

                int32_t oldFirst2Bytes = machine->memory[baseAddress / WORD_SIZE] & 0xFFFF0000;
                int32_t oldLast2Bytes = machine->memory[(baseAddress / WORD_SIZE) + 1] & TWO_BYTE_MASK;

                machine->memory[baseAddress / WORD_SIZE] = oldFirst2Bytes | ((uint32_t) newReplacementFirst2Bytes >> TWO_BYTE_SHIFT);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = oldLast2Bytes | (newReplacementLast2Bytes << TWO_BYTE_SHIFT); 
                break;
            case 3: ;
                int32_t newReplacement1Byte = toLittleEndian(machine->registers[rd]) & 0xFF000000;
                int32_t newReplacement3Bytes = toLittleEndian(machine->registers[rd]) & THREE_BYTE_MASK;

                int32_t old3Bytes = machine->memory[baseAddress / WORD_SIZE] & 0xFFFFFF00;
                int32_t oldByte = machine->memory[(baseAddress / WORD_SIZE) + 1] & EIGHT_BIT_MASK;

                machine->memory[baseAddress / WORD_SIZE] = old3Bytes | ((uint32_t) newReplacement1Byte >> THREE_BYTE_SHIFT);
                machine->memory[(baseAddress / WORD_SIZE) + 1] = oldByte | (newReplacement3Bytes << ONE_BYTE_SHIFT);
                break;
    }

    machine->registers[rn] = address;
}
