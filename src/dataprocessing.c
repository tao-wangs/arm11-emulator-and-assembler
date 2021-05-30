#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "dataprocessing.h"
#include "decode.h"

// main method for executing a data processing instruction
void dataProcessingInstruction(int instruction, ARM_STATE *machinePtr) {

	int condCode = (unsigned int) instruction >> COND_SHIFT;
	int immOperand = (instruction >> IMM_SHIFT) & ONE_BIT_MASK;
	int opcode = (instruction >> OPCODE_SHIFT) & FOUR_BIT_MASK;
	int setFlags = (instruction >> SET_SHIFT) & ONE_BIT_MASK;
	int rn = (instruction >> RN_SHIFT) & FOUR_BIT_MASK;
	int rd = (instruction >> RD_SHIFT) & FOUR_BIT_MASK;
	uint32_t operand2 = instruction & TWELVE_BIT_MASK;

	if (conditionMet(condCode, machinePtr)) {

		if (immediateOperandBitIsSet(immOperand)) {
			//printf("1");
			int rotateAmt = operand2 >> 8;
			int immediate = operand2 & EIGHT_BIT_MASK;
			operand2 = rotateRight(immediate, 2 * rotateAmt);
		} else {
			//printf("0");
			int rm = operand2 & FOUR_BIT_MASK;
			int shift = operand2 >> 4;
			//if ((shift & 0x1) == 0) {
			operand2 = shiftByConst(rm, shift, setFlags, machinePtr);
			//}
		}

		int res;
		int carryout = 0;
		int* carryptr = &carryout;

		switch (opcode) {
			case AND:
				res = executeAND(rn, operand2, rd, machinePtr);
				break;
			case EOR:
				res = executeEOR(rn, operand2, rd, machinePtr);
				break;
			case SUB:
				res = executeSUB(rn, operand2, rd, machinePtr, carryptr);
				break;
			case RSB:
				res = executeRSB(rn, operand2, rd, machinePtr, carryptr);
				break;
			case ADD:
				res = executeADD(rn, operand2, rd, machinePtr, carryptr);
				break;
			case TST:
				res = executeTST(rn, operand2, rd, machinePtr);
				break;
			case TEQ:
				res = executeTEQ(rn, operand2, rd, machinePtr);
				break;
			case CMP:
				res = executeCMP(rn, operand2, rd, machinePtr, carryptr);
				break;
			case ORR:
				res = executeORR(rn, operand2, rd, machinePtr);
				break;
			case MOV:
				executeMOV(operand2, rd, machinePtr);
				break;
			default:
				//printf("Opcode of instruction not recognised.");
				return;
		}

		if (conditionCodeIsSet(setFlags)) {
			updateFlags(opcode, res, carryout, machinePtr);
		}

	//printf("The result is %u\n", res);

	} else {
		//printf("Condition not met.\n");
	}
}

//Sets the CPSR's flags based on the result and the carryout of the operation
void updateFlags(int opcode, int res, int carryout, ARM_STATE *machinePtr) {

	machinePtr->registers[CPSR] |= (res & N_MASK);

	if (res == 0) {
		machinePtr->registers[CPSR] |= Z_MASK;
	} else {
		machinePtr->registers[CPSR] &= ~Z_MASK;
	}

	if (operationIsArithmetic(opcode) && carryout) {
		machinePtr->registers[CPSR] |= C_MASK;
	} else {
		machinePtr->registers[CPSR] &= ~C_MASK;
	}
}

// Checks whether the immediate operand bit is set to 1 or not.
int immediateOperandBitIsSet(int immOperand) {
	return immOperand == 1;
}

// Check whether the flags condition code is set.
int conditionCodeIsSet(int setFlags) {
	return setFlags == 1;
}

// Check whether the operation is arithmetic
int operationIsArithmetic(int opcode) {
	return (opcode == SUB || opcode == RSB || opcode == ADD || opcode == CMP);
}

// Check whether the operation is logical
int operationIsLogic(int opcode) {
	return (opcode == AND || opcode == EOR || opcode == TEQ || opcode == TST || opcode == MOV);
}

int executeAND(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = machinePtr->registers[rn] & operand2;
	return machinePtr->registers[rd];
}

int executeEOR(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = machinePtr->registers[rn] ^ operand2;
	return machinePtr->registers[rd];
}

int executeSUB(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr, int* carryptr) {
	int res = machinePtr->registers[rn] + ~operand2 + 1;
	*carryptr = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[rd] = machinePtr->registers[rn] - operand2;
	return machinePtr->registers[rd];
}

int executeRSB(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr, int* carryptr) {
	int res = operand2 + ~(machinePtr->registers[rn]) + 1;
	*carryptr = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[rd] = operand2 - machinePtr->registers[rn];
	return machinePtr->registers[rd];
}

int executeADD(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr, int* carryptr) {
	uint res = machinePtr->registers[rn] + operand2;
	*carryptr = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[rd] = res;
	return machinePtr->registers[rd];
}

int executeTST(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[rn] & operand2;
}

int executeTEQ(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[rn] ^ operand2;
}

int executeCMP(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr, int* carryptr) {
	int res = machinePtr->registers[rn] + ~operand2 + 1;
	*carryptr = (res > INT_MAX) ? 1 : 0;
	return machinePtr->registers[rn] - operand2;
}

int executeORR(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = machinePtr->registers[rn] | operand2;
	return machinePtr->registers[rd];
}

void executeMOV(uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = operand2;
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

//Rotates a binary string by a specified amount
uint32_t rotateRight(uint32_t operand2 , int rotateAmt) {
	int rotated = operand2 >> rotateAmt | operand2 << (REG_SIZE - rotateAmt);
	return rotated;
}

//Shifts the value in register rm by a specified amount in a specified way (left, right, arithmetic etc)
int shiftByConst(int rm, int shift, int setFlags, ARM_STATE *ptr) {

	int carryout;

	int bit4 = shift & 0x01;
	int rs = (unsigned int) shift >> 4;

	int val = ptr->registers[rm];
	
	int amt = bit4 == 1 ? rs & 0x000000FF : (unsigned int) shift >> 3;
	int type = ((unsigned int) shift >> 1) & 0x3;

	switch(type) {
		case LSL:
			carryout = (val >> (REG_SIZE - amt)) & ONE_BIT_MASK;
			val <<= amt;
			break;
		case LSR:
			carryout = (val >> amt) & ONE_BIT_MASK;
			val = (uint) val >> amt;
			break;
		case ASR:
			carryout = (val >> amt) & ONE_BIT_MASK;
			val >>= amt;
			break;
		case ROR:
			carryout = (val >> amt) & ONE_BIT_MASK;
			val = rotateRight(val, amt);
			break;
	}
	
	if (conditionCodeIsSet(setFlags)) {
		if (carryout) {
			ptr->registers[CPSR] |= C_MASK;
		} else {
			ptr->registers[CPSR] &= ~C_MASK;
		}
	}
	return val;
}
