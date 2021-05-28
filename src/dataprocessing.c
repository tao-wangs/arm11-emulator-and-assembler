#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "dataprocessing.h"
#include "decode.h"

// main method for executing a data processing instruction
void dataProcessingInstruction(char *instruction, ARM_STATE *machinePtr) {

	int cond_shift = 28;
	int imm_shift = 25;
	int opcode_shift = 21;
	int set_shift = 20;
	int rn_shift = 16;
	int rd_shift = 12;
	
	int instructionAsInt = binConverter(instruction);
	
	int condCode = instructionAsInt >> cond_shift;
	int immOperand = (instructionAsInt >> imm_shift) & ONE_BIT_MASK;
	int opcode = (instructionAsInt >> opcode_shift) & FOUR_BIT_MASK;
	int setFlags = (instructionAsInt >> set_shift) & ONE_BIT_MASK;
	int rn = (instructionAsInt >> rn_shift) & FOUR_BIT_MASK;
	int rd = (instructionAsInt >> rd_shift) & FOUR_BIT_MASK;
	uint32_t operand2 = instructionAsInt & TWELVE_BIT_MASK;

	if (conditionMet(condCode, machinePtr)) {

		if (immediateOperandBitIsSet(immOperand)) {
			printf("1");
			int rotateAmt = operand2 >> 8;
			operand2 = rotateRight(operand2, 2 * rotateAmt);
		} else {
			printf("0");
			int rm = operand2 & FOUR_BIT_MASK;
			int shift = operand2 >> 4; 

			if ((shift & 0x1) == 0) {
				operand2 = shiftByConst(rm, shift, machinePtr);
			}
		}

		int res;
		int carryout = 0;

		switch (opcode) {
			case and:
				res = executeAND(rn, operand2, rd, machinePtr);
				break;
			case eor:
				res = executeEOR(rn, operand2, rd, machinePtr);
				break;
			case sub:
				res = executeSUB(rn, operand2, rd, machinePtr, carryout);
				break;
			case rsb:
				res = executeRSB(rn, operand2, rd, machinePtr, carryout);
				break;
			case add:
				res = executeADD(rn, operand2, rd, machinePtr, carryout);
				break;
			case tst:
				res = executeTST(rn, operand2, rd, machinePtr);
				break;
			case teq:
				res = executeTEQ(rn, operand2, rd, machinePtr);
				break;
			case cmp:
				res = executeCMP(rn, operand2, rd, machinePtr, carryout);
				break;
			case orr:
				res = executeORR(rn, operand2, rd, machinePtr);
				break;
			case mov:
				executeMOV(operand2, rd, machinePtr);
				break;
			default:
				printf("Opcode of instruction not recognised.");
				return;
		}

		if (conditionCodeIsSet(setFlags)) {
			updateFlags(opcode, res, carryout, machinePtr);
		}

	printf("The result is %u\n", res);

	} else {
		printf("Condition not met.");
	}
}

//Sets the CPSR's flags based on the result and the carryout of the operation
void updateFlags(int opcode, int res, int carryout, ARM_STATE *machinePtr) {
	
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
int immediateOperandBitIsSet(int immOperand) {
	return immOperand == 1;
}

// Check whether the flags condition code is set.
int conditionCodeIsSet(int setFlags) {
	return setFlags == 1;
}	

// Check whether the operation is arithmetic
int operationIsArithmetic(int opcode) {
	return (opcode == sub || opcode == rsb || opcode == add || opcode == cmp);
}

// Check whether the operation is logical
int operationIsLogic(int opcode) {
	return (opcode == and || opcode == eor || opcode == teq || opcode == tst || opcode == mov);
}

int executeAND(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = machinePtr->registers[rn] & operand2;
	return machinePtr->registers[rd];
}

int executeEOR(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = machinePtr->registers[rn] ^ operand2;
	return machinePtr->registers[rd];
}

int executeSUB(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr, int carryout) {
	int res = machinePtr->registers[rn] + ~operand2 + 1;
	carryout = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[rd] = machinePtr->registers[rn] - operand2;
	return machinePtr->registers[rd];
}

int executeRSB(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr, int carryout) {
	int res = operand2 + ~(machinePtr->registers[rn]) + 1;
	carryout = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[rd] = operand2 - machinePtr->registers[rn];
	return machinePtr->registers[rd];
}

int executeADD(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr, int carryout) {
	uint res = machinePtr->registers[rn] + operand2;
	carryout = (res > INT_MAX) ? 1 : 0;
	machinePtr->registers[rd] = res;
	return machinePtr->registers[rd];
}
		
int executeTST(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[rn] & operand2;
}

int executeTEQ(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr) {
	return machinePtr->registers[rn] ^ operand2;
}

int executeCMP(int rn, uint32_t operand2 , int rd, ARM_STATE *machinePtr, int carryout) {
	int res = machinePtr->registers[rn] + ~operand2 + 1;
	carryout = (res > INT_MAX) ? 1 : 0;
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
	int rotated = operand2 >> rotateAmt | operand2 << (32 - rotateAmt);
	return rotated;
}

//Shifts the value in register rm by a specified amount in a specified way (left, right, arithmetic etc)
int shiftByConst(int rm, int shift, ARM_STATE *ptr) {

	int val = ptr->registers[rm];
	int carryout;
	int amt = shift >> 3;
	int type = (shift >> 1) & 2;

	switch(type) {
		case 0:
			carryout = (val >> (REG_SIZE - amt)) & ONE_BIT_MASK;
			val = (uint) val << amt;
			break;
		case 1:
			carryout = (val >> amt) & ONE_BIT_MASK;
			val = (uint) val >> amt;
			break;
		case 2:
			carryout = (val >> amt) & ONE_BIT_MASK;
			val = val >> amt;
			break;
		case 3:
			carryout = (val >> amt) & ONE_BIT_MASK;
			val = rotateRight(val, amt);
			break;
	}

	if (carryout) {
		ptr->registers[CPSR] |= C_mask;
	} else {
		ptr->registers[CPSR] &= ~C_mask;
	}

	return val;
}