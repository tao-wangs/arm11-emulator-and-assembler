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

#define C_MASK 0b00100000000000000000000000000000

void dataProcessingInstruction(char *instruction, ARM_STATE *machinePtr);

int immediateOperandBitIsSet(char *immediateOperand);
int conditionCodeIsSet(char *setConditionCode);
int operationIsArithmetic(char *opcode);
int operationIsLogic(char *opcode);

int executeAND(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
int executeEOR(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
int executeSUB(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
int executeRSB(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
int executeADD(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
int executeTST(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
int executeTEQ(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
int executeCMP(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
int executeORR(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr);
void executeMOV(char *operand2, char *rd, ARM_STATE *machinePtr);

int binConverter(char *str);
char *zeroExtend(char *operand2);
char *rotateRight(char *operand2, int rotateAmt);
char *shiftByConst(char *rm, char *shift, ARM_STATE *ptr);

#endif