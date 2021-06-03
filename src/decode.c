#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "decode.h"
#include "utility.h"

//decides whether an instruction is one of the four instruction types;
INSTRUCTION_TYPE decode(uint32_t instruction) {
    uint32_t temp = (instruction >> TEMP_SHIFT) & TWO_BIT_MASK;
    uint32_t temp2 = (instruction & MASK_2) >> TEMP2_SHIFT;

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
