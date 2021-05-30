#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "decode.h"

//decides whether an instruction is one of the four instruction types;
INSTRUCTION_TYPE decode(unsigned int instruction) {
    unsigned int mask1 = 0x0C000000;
    unsigned int mask2 = 0x0FC000F0;
    unsigned int haltMask = 0x00000000;
    unsigned int temp = (instruction & mask1) >> 26;
    unsigned int temp2 = (instruction & mask2) >> 4;

    if(!(instruction | haltMask)){
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
    int nMask = 0x8000000;
    int zMask = 0x4000000;
    int vMask = 0x1000000;

    unsigned char n = (machine->registers[CPSR] & nMask) >> 31; 
    unsigned char z = (machine->registers[CPSR] & zMask) >> 30; 
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

int toLittleEndian(int word) {
    int byte1 = (word & 0x000000FF); //01 
    int byte2 = (word & 0x0000FF00) >> 8; //10
    int byte3 = (word & 0x00FF0000) >> 16; //a0
    int byte4 = (word & 0xFF000000) >> 24; //e3
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4; //it is now 01 10 a0 e3
}
