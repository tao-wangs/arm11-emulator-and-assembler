#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include "utility.h"

typedef enum {
	AND = 0x0,
	EOR = 0x1,
	SUB = 0x2,
	RSB = 0x3,
	ADD = 0x4,
	TST = 0x8,
	TEQ = 0x9,
	CMP = 0xA,
	ORR = 0xC,
	MOV = 0xD
} DP_INSTR_TYPE;

#define TWELVE_BIT_MASK 0xFFF

#define INT_MAX 0xFFFFFFFF
#define REG_SIZE 32

#define OPCODE_SHIFT 21
#define RN_SHIFT_DP 16
#define RD_SHIFT_DP 12
#define RS_SHIFT_DP 4

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

int32_t shiftByConst(int32_t rm, int32_t shift, int32_t setFlags, ARM_STATE *ptr);
void updateFlags(int32_t opcode, int32_t res, int32_t carryout, ARM_STATE *ptr);

#endif
