#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "decode.h"

//decides whether an instruction is one of the four instruction types;
INSTRUCTION_TYPE decode(uint32_t instruction) {
    uint32_t temp = (instruction & MASK_1) >> 26;
    uint32_t temp2 = (instruction & MASK_2) >> 4;

    if(!(instruction | HALT_MASK)){
    	return Halt;
    }

    switch (temp)
    {
        case 0:
            if (temp2 == 9) {
                return Multiply;
            }
            return DataProcessing;
        case 1:
            return SingleDataTransfer;
        case 2:
            return Branch;
        default:
            assert(temp == 3);
	    printf("error in decoding instruction..");
	    exit(1); //throw an error because 0b11 isn't used so the instruction must be invalid
    }
}

/* flags in CPSR:
n = last result was negative
z = last result was zero
c = last result caused a bit to be carried out
v = last result overflowed */
bool conditionMet(uint32_t conditionCode, ARM_STATE *machine) {

    unsigned char n = (machine->registers[CPSR] & N_MASK) >> 31;
    unsigned char z = (machine->registers[CPSR] & Z_MASK) >> 30;
    unsigned char v = (machine->registers[CPSR] & V_MASK) >> 28;

    switch (conditionCode) {
        case EQ:
            return z;
        case NE:
            return !z;
        case GE:
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

int32_t toLittleEndian(int32_t word) {
    int32_t byte1 = (word & 0x000000FF); //01
    int32_t byte2 = (word & 0x0000FF00) >> 8; //10
    int32_t byte3 = (word & 0x00FF0000) >> 16; //a0
    int32_t byte4 = (word & 0xFF000000) >> 24; //e3
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4; //it is now 01 10 a0 e3
}
