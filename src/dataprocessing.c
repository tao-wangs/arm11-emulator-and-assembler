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
	
	int condMask = 11110000000000000000000000000000;
	int cond_shift = 28;
	int immediateMask = 10000000000000000000000000;
	int immediate_shift = 25;
	int opcodeMask = 1111000000000000000000000;
	int opcode_shift = 21;
	int setMask = 100000000000000000000;
	int set_shift = 20;
	int rnMask = 11110000000000000000;
	int rn_shift = 16;
	int rdMask = 1111000000000000;
	int rd_shift = 12;
	int operand2Mask = 111111111111;
	
	
	uint32_t instructionAsInt = atoi(instruction);
	
	int cond = (instructionAsInt & condMask) << cond_shift;
	int immediateOperand = (instructionAsInt & immediateMask) << immediate_shift; 
	int opcode = (instructionAsInt & opcodeMask) << opcode_shift;
	int setConditionCode = (instructionAsInt & setMask) << set_shift;
	int rn = (instructionAsInt & rnMask) << rn_shift;
	int rd = (instructionAsInt & rdMask) << rd_shift;
	int operand2 = (instructionAndInt & operand2Mask);
	
	strncpy(cond, instruction, 4);
	strncpy(immediateOperand, instruction+6, 1);
	strncpy(opcode, instruction+7, 4);
	strncpy(setConditionCode, instruction+11, 1);
	strncpy(rn, instruction+12, 4);
	strncpy(rd, instruction+16, 4);
	strncpy(operand2, instruction+20, 12);

	if (immediateOperandBitIsSet(immediateOperand)) {
		printf("1");
		char *rotateAmt = malloc(4);
		strncpy(rotateAmt, operand2, 4);
		operand2 = zeroExtend(operand2+4);
		operand2 = rotateRight(operand2, 2 * binConverter(rotateAmt));
	} else {
		printf("0");
		char *rm = malloc(4);
		char *shift = malloc(8);
		strncpy(rm, operand2+8, 4);
		strncpy(shift, operand2, 8);

		if (shift[7] == '0') {
			operand2 = shiftByConst(rm, shift, machinePtr);
		}
	}
	
	int res = 0;
	int carryout = 0;
	
	switch (binConverter(opcode)) {
		case 0:
			res = executeAND(rn, operand2, rd, machinePtr);
			break;
		case 1:
			res = executeEOR(rn, operand2, rd, machinePtr);
			break;
		case 2:
			res = executeSUB(rn, operand2, rd, machinePtr, carryout);
			break;
		case 3:
			res = executeRSB(rn, operand2, rd, machinePtr, carryout);
			break;
		case 4:
			res = executeADD(rn, operand2, rd, machinePtr, carryout);
			break;
		case 8:
			res = executeTST(rn, operand2, rd, machinePtr);
			break;
		case 9:
			res = executeTEQ(rn, operand2, rd, machinePtr);
			break;
		case 10:
			res = executeCMP(rn, operand2, rd, machinePtr, carryout);
			break;
		case 12:
			res = executeORR(rn, operand2, rd, machinePtr);
			break;
		case 13:
			executeMOV(operand2, rd, machinePtr);
			break;
	}

	if (conditionCodeIsSet(setConditionCode)) {
		updateFlags(opcode, res, carryout, machinePtr);
	}

	printf("The result is: %u\n", res);
}

//Sets the CPSR's flags based on the result and the carryout of the operation
void updateFlags(char *opcode, int res, int carryout, ARM_STATE *machinePtr) {
	
	machinePtr->registers[CPSR] |= (res & N_mask);

	if (res == 0) {
		machinePtr->registers[CPSR] |= Z_mask;
	} else {
		machinePtr->registers[CPSR] &= ~Z_mask;
	}

	if (operationIsArithmetic(opcode) && carryout) {
		machinePtr->registers[CPSR] |= C_mask;
	} else {
		machinePtr->registers[CPSR] &= ~C_mask;
	}
}

// Checks whether the immediate operand bit is set to 1 or not.
int immediateOperandBitIsSet(char *immediateOperand) {
	return strcmp(immediateOperand, "1") == 0;
	// return immediateOperand == 1;
}

// Check whether the flags condition code is set.
int conditionCodeIsSet(char *setConditionCode) {
	return strcmp(setConditionCode, "1") == 0;
	// return setConditionCode == 1;
}	

// Check whether the operation is arithmetic
int operationIsArithmetic(char *opcode) {
	return !(strcmp(opcode, sub) && 
			strcmp(opcode, rsb) && 
			strcmp(opcode, add) && 
			strcmp(opcode, cmp));
	// return (opcode == sub || opcode == rsb || opcode == add || opcode == cmp); 
	// Would need to change the constants to their denary value, and not leave them in binary in the header file.
}

// Check whether the operation is logical
int operationIsLogic(char *opcode) {
	return !(strcmp(opcode, and) &&
			strcmp(opcode, eor) &&
			strcmp(opcode, orr) &&
			strcmp(opcode, teq) &&
			strcmp(opcode, tst) &&
			strcmp(opcode, mov));
	// return (opcode == and || opcode == eor || opcode == teq || opcode == tst || opcode == mov);
	// Would need to change the constants to their denary value, and not leave them in binary in the header file.
}

int executeAND(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = machinePtr->registers[binConverter(rn)] & binConverter(operand2);
	return machinePtr->registers[binConverter(rd)];
	// 	machinePtr->registers[rd] = machinePtr->registers[rn] & operand2;
	// 	Here don't we need operand2 in its binary form though to be able to and with it effectively?
	//	return machinePtr->registers[rd];
}

int executeEOR(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = machinePtr->registers[binConverter(rn)] ^ binConverter(operand2);
	return machinePtr->registers[binConverter(rd)];
}

int executeSUB(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr, int carryout) {
	int res = machinePtr->registers[binConverter(rn)] + ~(binConverter(operand2)) + 1;
	carryout = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[binConverter(rd)] = res;
	return machinePtr->registers[binConverter(rd)];
}

int executeRSB(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr, int carryout) {
	int res = binConverter(operand2) + ~(machinePtr->registers[binConverter(rn)]) + 1;
	carryout = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[binConverter(rd)] = res;
	return machinePtr->registers[binConverter(rd)];
}

int executeADD(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr, int carryout) {
	uint res = machinePtr->registers[binConverter(rn)] + binConverter(operand2);
	carryout = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[binConverter(rd)] = res;
	return machinePtr->registers[binConverter(rd)];
}
		
int executeTST(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[binConverter(rn)] & binConverter(operand2);
}

int executeTEQ(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[binConverter(rn)] ^ binConverter(operand2);
}

int executeCMP(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr, int carryout) {
	int res = machinePtr->registers[binConverter(rn)] + ~(binConverter(operand2)) + 1;
	carryout = (res > INT_MAX) ? 1 : 0;
	return machinePtr->registers[binConverter(rn)] - binConverter(operand2);	
}

int executeORR(char *rn, char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = machinePtr->registers[binConverter(rn)] | binConverter(operand2);
	return machinePtr->registers[binConverter(rd)];
}

void executeMOV(char *operand2, char *rd, ARM_STATE *machinePtr) {
	machinePtr->registers[binConverter(rd)] = binConverter(operand2);
}

//Converts a binary string into its denary value
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

//Zero extends a binary string to 32 bits
char *zeroExtend(char *operand2) {
	char *ptr = malloc(32);
	int zeroLen = 32 - strlen(operand2);

	memset(ptr, '0', zeroLen);
	memcpy(ptr + zeroLen, operand2, strlen(operand2));

	return ptr;
}

//Rotates a binary string by a specified amount
char *rotateRight(char *operand2, int rotateAmt) {
	int num = binConverter(operand2);

	int rotated = num >> rotateAmt | num << (32 - rotateAmt);

	return binRep(rotated);
}

//Shifts the value in register rm by a specified amount in a specified way (left, right, arithmetic etc)
char *shiftByConst(char *rm, char *shift, ARM_STATE *ptr) {

	int val = ptr->registers[binConverter(rm)];
	char *type = malloc(2);
	char *shiftAmt = malloc(5);
	int carryout;
	char *res;

	strncpy(shiftAmt, shift, 5);
	strncpy(type, shift+5, 2);

	int amt = binConverter(shiftAmt);
	char *binReg = binRep(val);

	switch(binConverter(type)) {
		case 0:
			carryout = (binReg[amt - 1] - '0');
			val = val << amt;
			res = binRep(val);
			break;
		case 1:
			carryout = (binReg[32 - amt] - '0');
			val = val >> amt;
			res = binRep(val);
			break;
		case 2:
			carryout = (binReg[32 - amt] - '0');
			val = val >> amt;
			res = binRep(val);
			memset(res, res[0], amt + 1);
			break;
		case 3:
			carryout = (binReg[32 - amt] - '0');
			res = rotateRight(binReg, amt);
			break;
	}

	if (carryout) {
		ptr->registers[CPSR] |= C_mask;
	} else {
		ptr->registers[CPSR] &= ~C_mask;
	}

	return res;
}
