#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

enum INSTRUCTION_TYPE {DataProcessing, Multiply, SingleDataTransfer, Branch};

enum INSTRUCTION_TYPE decode(unsigned int instruction) {
    unsigned int mask1 = 0x0C000000;
    unsigned int mask2 = 0x0FC000F0;
    unsigned int temp = (instruction & mask1) >> 26;
    unsigned int temp2 = (instruction & mask2) >> 4;
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
            //error
    }
}