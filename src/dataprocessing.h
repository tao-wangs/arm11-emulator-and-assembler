#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include "arm_state.h"

#define eq "0000"
#define ne "0001"
#define ge "1010"
#define lt "1011"
#define gt "1100"
#define le "1101"
#define al "1110"

#define and "0000"
#define eor "0001"
#define sub "0010"
#define rsb "0011"
#define add "0100"
#define tst "1000"
#define teq "1001"
#define cmp "1010"
#define orr "1100"
#define mov "1101"

#define N_flag "1000"
#define Z_flag "0100"
#define C_flag "0010"
#define V_flag "0001"

#define N_mask 0x80000000
#define Z_mask 0x40000000
#define C_mask 0x20000000
#define V_mask 0x10000000

#define INT_MAX 0xFFFFFFFF

void dataProcessingInstruction(char *instruction, ARM_STATE *machinePtr);

int binConverter(char *str);
char *zeroExtend(char *operand2);
char *rotateRight(char *operand2, int rotateAmt);
char *shiftByConst(char *rm, char *shift, ARM_STATE *ptr);
void updateFlags(char *opcode, int res, int carryout, ARM_STATE *ptr);

#endif