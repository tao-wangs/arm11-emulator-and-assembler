#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataprocessing.h"
#include "arm_state.h"
#include "binreader.h"

// main method for executing a data processing instruction
void dataProcessingInstruction(char *instruction, ARM_STATE *machinePtr) {

	char *cond = malloc(4);
	char *immediateOperand = malloc(1);
	char *opcode = malloc(4);
	char *setConditionCode = malloc(1);
	char *rn = malloc(4);
	char *rd = malloc(4);
	char *operand2 = malloc(12);
	
	strncpy(cond, instruction, 4);
	strncpy(immediateOperand, instruction+6, 1);
	strncpy(opcode, instruction+7, 4);
	strncpy(setConditionCode, instruction+11, 1);
	strncpy(rn, instruction+12, 4);
	strncpy(rd, instruction+16, 4);
	strncpy(operand2, instruction+20, 12);

	if (immediateOperandBitIsSet) {
		char *rotateAmt;
		strncpy(rotateAmt, operand2, 4);
		operand2 = zeroExtend(operand2+4);
		operand2 = rotate(2 * binConverter(rotateAmt), operand2);
	}
	
	int res;
	
	switch (binConverter(opcode)) {
		case 0:
			res = executeAND(rn, operand2, rd, machinePtr);
			break;
		case 1:
			res = executeEOR(rn, operand2, rd, machinePtr);
			break;
		case 2:
			res = executeSUB(rn, operand2, rd, machinePtr);
			break;
		case 3:
			res = executeRSB(rn, operand2, rd, machinePtr);
			break;
		case 4:
			res = executeADD(rn, operand2, rd, machinePtr);
			break;
		case 8:
			res = executeTST(rn, operand2, rd, machinePtr);
			break;
		case 9:
			res = executeTEQ(rn, operand2, rd, machinePtr);
			break;
		case 10:
			res = executeCMP(rn, operand2, rd, machinePtr);
			break;
		case 12:
			res = executeORR(rn, operand2, rd, machinePtr);
			break;
		case 13:
			executeMOV(operand2, rd, machinePtr);
			break;
		default:
			break;
	}
}

// Checks whether the immediate operand bit is set to 1 or not.
int immediateOperandBitIsSet(char *immediateOperand) {
	return strcmp(immediateOperand, "1") == 0;
}

// Check whether the flags condition code is set.
int conditionCodeIsSet(char *setConditionCode) {
	return strcmp(setConditionCode, "1") == 0;
}	

// Check whether the operation is arithmetic
int operationIsArithmetic(char *opcode) {
	return !(strcmp(opcode, sub) && 
			strcmp(opcode, rsb) && 
			strcmp(opcode, add) && 
			strcmp(opcode, cmp));
}

// Check whether the operation is logical
int operationIsLogic(char *opcode) {
	return !(strcmp(opcode, and) &&
			strcmp(opcode, eor) &&
			strcmp(opcode, orr) &&
			strcmp(opcode, teq) &&
			strcmp(opcode, tst) &&
			strcmp(opcode, mov));
}

int executeAND(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = machinePtr->registers[binConverter(rn)] & binConverter(operand2);
	return machinePtr->registers[binConverter(rd)];
}

int executeEOR(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = machinePtr->registers[binConverter(rn)] ^ binConverter(operand2);
	return machinePtr->registers[binConverter(rd)];
}

int executeSUB(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = machinePtr->registers[binConverter(rn)] - binConverter(operand2);
	return machinePtr->registers[binConverter(rd)];
}

int executeRSB(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = binConverter(operand2) - machinePtr->registers[binConverter(rn)];
	return machinePtr->registers[binConverter(rd)];
}

int executeADD(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = machinePtr->registers[binConverter(rn)] + binConverter(operand2);
	return machinePtr->registers[binConverter(rd)];
}
		
int executeTST(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[binConverter(rn)] & binConverter(operand2);
}

int executeTEQ(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[binConverter(rn)] ^ binConverter(operand2);
}

int executeCMP(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[binConverter(rn)] - binConverter(operand2);	
}

int executeORR(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = machinePtr->registers[binConverter(rn)] | binConverter(operand2);
	return machinePtr->registers[binConverter(rd)];
}

void executeMOV(char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = binConverter(operand2);
}

int binConverter(char *str) {
	int res = 0;
	int cnt = 1;
	int len = strlen(str);

	for (int i = 0; i < len; i++) {
		if (str[(len - 1) - i] == '1') {
			res += cnt;
		}

		cnt *= 2; 
	}

	return res;
}

char *zeroExtend(char *operand2) {
	char *ptr = malloc(32);
	int zeroLen = 32 - strlen(operand2);

	memset(ptr, '0', zeroLen);
	memcpy(ptr + zeroLen, operand2, strlen(operand2));

	return ptr;
}

char *rotate(int rotateAmt, char *operand2) {
	int num = binConverter(operand2);

	int rotated = num >> rotateAmt | num << (32 - rotateAmt);

	return binRep(rotated);
}