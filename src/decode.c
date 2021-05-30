#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "decode.h"

//decides whether an instruction is one of the four instruction types;
INSTRUCTION_TYPE decode(unsigned int instruction) {
    unsigned int temp = (instruction & MASK_1) >> 26;
    unsigned int temp2 = (instruction & MASK_2) >> 4;

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
bool conditionMet(unsigned int conditionCode, ARM_STATE *machine) {

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
