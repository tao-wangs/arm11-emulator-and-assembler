#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include "arm_state.h"

#define eq 0b0000
#define ne 0b0001
#define ge 0b1010
#define lt 0b1011
#define gt 0b1100
#define le 0b1101
#define al 0b1110

#define and 0b0000
#define eor 0b0001
#define sub 0b0010
#define rsb 0b0011
#define add 0b0100
#define tst 0b1000
#define teq 0b1001
#define cmp 0b1010
#define orr 0b1100
#define mov 0b1101

#define FOUR_BIT_MASK 0xF
#define ONE_BIT_MASK 0x1
#define TWELVE_BIT_MASK 0xFFF
#define N_mask 0x80000000
#define Z_mask 0x40000000
#define C_mask 0x20000000
#define V_mask 0x10000000

#define INT_MAX 0xFFFFFFFF
#define REG_SIZE 32

void dataProcessingInstruction(char *instruction, ARM_STATE *machinePtr);

int immediateOperandBitIsSet(int immOperand);
int conditionCodeIsSet(int setFlags);
int operationIsArithmetic(int opcode);
int operationIsLogic(int opcode);

int executeAND(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr);
int executeEOR(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr);
int executeSUB(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr, int carryout);
int executeRSB(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr, int carryout);
int executeADD(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr, int carryout);
int executeTST(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr);
int executeTEQ(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr);
int executeCMP(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr, int carryout);
int executeORR(int rn, u_int32_t operand2, int rd, ARM_STATE *machinePtr);
void executeMOV(u_int32_t operand2, int rd, ARM_STATE *machinePtr);

int binConverter(char *str);
int zeroExtend(int operand2);
u_int32_t rotateRight(u_int32_t operand2, int rotateAmt);
int shiftByConst(int rm, int shift, ARM_STATE *ptr);
void updateFlags(int opcode, int res, int carryout, ARM_STATE *ptr);
bool conditionMet(unsigned int condCode, ARM_STATE *ptr);

#endif
