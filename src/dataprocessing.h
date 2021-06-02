#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include <stdint.h>
#include <stdbool.h>

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
#define MAX_VAL 0xffffffff

#define COND_SHIFT 28
#define IMM_SHIFT 25
#define OPCODE_SHIFT 21
#define SET_SHIFT 20
#define RN_SHIFT 16
#define RD_SHIFT 12

void dataProcessingInstruction(int32_t instruction, ARM_STATE *machinePtr);

int32_t immediateOperandBitIsSet(int32_t immOperand);
int32_t conditionCodeIsSet(int32_t setFlags);
int32_t operationIsArithmetic(int32_t opcode);
int32_t operationIsLogic(int32_t opcode);

int32_t executeAND(int32_t rn, uint32_t operand2, int32_t rd, ARM_STATE *machinePtr);
int32_t executeEOR(int32_t rn, uint32_t operand2, int32_t rd, ARM_STATE *machinePtr);
int32_t executeSUB(int32_t rn, uint32_t operand2, int32_t rd, ARM_STATE *machinePtr, int32_t* carryptr);
int32_t executeRSB(int32_t rn, uint32_t operand2, int32_t rd, ARM_STATE *machinePtr, int32_t* carryptr);
int32_t executeADD(int32_t rn, uint32_t operand2, int32_t rd, ARM_STATE *machinePtr, int32_t* carryptr);
int32_t executeTST(int32_t rn, uint32_t operand2, ARM_STATE *machinePtr);
int32_t executeTEQ(int32_t rn, uint32_t operand2, ARM_STATE *machinePtr);
int32_t executeCMP(int32_t rn, uint32_t operand2, ARM_STATE *machinePtr, int32_t* carryptr);
int32_t executeORR(int32_t rn, uint32_t operand2, int32_t rd, ARM_STATE *machinePtr);
void executeMOV(uint32_t operand2, int32_t rd, ARM_STATE *machinePtr);

uint32_t rotateRight(uint32_t operand2, int rotateAmt);
int32_t shiftByConst(int32_t rm, int32_t shift, int32_t setFlags, ARM_STATE *ptr);
void updateFlags(int32_t opcode, int32_t res, int32_t carryout, ARM_STATE *ptr);

#endif
