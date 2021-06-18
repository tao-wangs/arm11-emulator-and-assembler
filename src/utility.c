
#include "utility.h"

bool conditionMet(uint32_t conditionCode, ARM_STATE *machine) {

    unsigned char n = (machine->registers[CPSR] >> N_SHIFT) & ONE_BIT_MASK;
    unsigned char z = (machine->registers[CPSR] >> Z_SHIFT) & ONE_BIT_MASK;
    unsigned char v = (machine->registers[CPSR] >> V_SHIFT) & ONE_BIT_MASK;

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
    int32_t byte1 = (word & EIGHT_BIT_MASK);
    int32_t byte2 = (word >> ONE_BYTE_SHIFT) & EIGHT_BIT_MASK; 
    int32_t byte3 = (word >> TWO_BYTE_SHIFT) & EIGHT_BIT_MASK; 
    int32_t byte4 = (word >> THREE_BYTE_SHIFT) & EIGHT_BIT_MASK; 
    return (byte1 << THREE_BYTE_SHIFT) | (byte2 << TWO_BYTE_SHIFT) | (byte3 << ONE_BYTE_SHIFT) | byte4;
}

uint32_t rotateRight(uint32_t value, int32_t shift) {
    if ((shift &= 31) == 0) {
      return value;
    }
    return (value >> shift) | (value << (32 - shift));
}

// Will return the int represented by the string, ignoring the r, #, or = at the start of the string.
int32_t stringToInt(char* opToken) {
    return (int32_t) strtol(strtok(opToken, "r#= "), NULL, 0);
}

