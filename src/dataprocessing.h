#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include <stdint.h>

#include "arm_state.h"

#define AND 0
#define EOR 1
#define SUB 2
#define RSB 3
#define ADD 4
#define TST 8
#define TEQ 9
#define CMP 10
#define ORR 12
#define MOV 13

#define FOUR_BIT_MASK 0xF
#define EIGHT_BIT_MASK 0xFF
#define ONE_BIT_MASK 0x1
#define TWELVE_BIT_MASK 0xFFF
#define N_MASK 0x80000000
#define Z_MASK 0x40000000
#define C_MASK 0x20000000
#define V_MASK 0x10000000

#define INT_MAX 0xFFFFFFFF
#define REG_SIZE 32

#define COND_SHIFT 28
#define IMM_SHIFT 25
#define OPCODE_SHIFT 21
#define SET_SHIFT 20
#define RN_SHIFT 16
#define RD_SHIFT 12

void dataProcessingInstruction(int instruction, ARM_STATE *machinePtr);

int immediateOperandBitIsSet(int immOperand);
int conditionCodeIsSet(int setFlags);
int operationIsArithmetic(int opcode);
int operationIsLogic(int opcode);

int executeAND(int rn, uint32_t operand2, int rd, ARM_STATE *machinePtr);
int executeEOR(int rn, uint32_t operand2, int rd, ARM_STATE *machinePtr);
int executeSUB(int rn, uint32_t operand2, int rd, ARM_STATE *machinePtr, int* carryptr);
int executeRSB(int rn, uint32_t operand2, int rd, ARM_STATE *machinePtr, int* carryptr);
int executeADD(int rn, uint32_t operand2, int rd, ARM_STATE *machinePtr, int* carryptr);
int executeTST(int rn, uint32_t operand2, ARM_STATE *machinePtr);
int executeTEQ(int rn, uint32_t operand2, ARM_STATE *machinePtr);
int executeCMP(int rn, uint32_t operand2, ARM_STATE *machinePtr, int* carryptr);
int executeORR(int rn, uint32_t operand2, int rd, ARM_STATE *machinePtr);
void executeMOV(uint32_t operand2, int rd, ARM_STATE *machinePtr);

int zeroExtend(int operand2);
uint32_t rotateRight(uint32_t operand2, int rotateAmt);
int shiftByConst(int rm, int shift, int setFlags, ARM_STATE *ptr);
void updateFlags(int opcode, int res, int carryout, ARM_STATE *ptr);
bool conditionMet(unsigned int condCode, ARM_STATE *ptr);

#endif
