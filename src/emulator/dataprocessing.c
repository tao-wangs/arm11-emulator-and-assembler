#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "dataprocessing.h"
#include "arm_state.h"
#include "../utility.h"

// main method for executing a data processing instruction
void dataProcessingInstruction(int32_t instruction, ARM_STATE *machinePtr) {

	int32_t condCode = (uint32_t) instruction >> CONDCODE_SHIFT;
	int32_t immOperand = (instruction >> IMM_OPERAND_SHIFT) & ONE_BIT_MASK;
	int32_t opcode = (instruction >> OPCODE_SHIFT) & FOUR_BIT_MASK;
	int32_t setFlags = (instruction >> SET_FLAGS_SHIFT) & ONE_BIT_MASK;
	int32_t rn = (instruction >> RN_SHIFT_DP) & FOUR_BIT_MASK;
	int32_t rd = (instruction >> RD_SHIFT_DP) & FOUR_BIT_MASK;
	uint32_t operand2 = instruction & TWELVE_BIT_MASK;

	if (conditionMet(condCode, machinePtr)) {

		if (immediateOperandBitIsSet(immOperand)) {
			int32_t rotateAmt = operand2 >> 8;
			int32_t immediate = operand2 & EIGHT_BIT_MASK;
			operand2 = rotateRight(immediate, 2 * rotateAmt);
		} else {
			int32_t rm = operand2 & FOUR_BIT_MASK;
			int32_t shift = operand2 >> 4;
			operand2 = shiftByConst(rm, shift, setFlags, machinePtr);
		}

		int32_t res;
		int32_t carryout = 0;
		int32_t* carryptr = &carryout;

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
				res = executeTST(rn, operand2, machinePtr);
				break;
			case TEQ:
				res = executeTEQ(rn, operand2, machinePtr);
				break;
			case CMP:
				res = executeCMP(rn, operand2, machinePtr, carryptr);
				break;
			case ORR:
				res = executeORR(rn, operand2, rd, machinePtr);
				break;
			case MOV:
				executeMOV(operand2, rd, machinePtr);
				break;
			default:
				return;
		}

		if (conditionCodeIsSet(setFlags)) {
			updateFlags(opcode, res, carryout, machinePtr);
		}
	}
}

//Sets the CPSR's flags based on the result and the carryout of the operation
void updateFlags(int32_t opcode, int32_t res, int32_t carryout, ARM_STATE *machinePtr) {

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
int32_t immediateOperandBitIsSet(int32_t immOperand) {
	return immOperand == 1;
}

// Check whether the flags condition code is set.
int32_t conditionCodeIsSet(int32_t setFlags) {
	return setFlags == 1;
}

// Check whether the operation is arithmetic
int32_t operationIsArithmetic(int32_t opcode) {
	return (opcode == SUB || opcode == RSB || opcode == ADD || opcode == CMP);
}

// Check whether the operation is logical
int32_t operationIsLogic(int32_t opcode) {
	return (opcode == AND || opcode == EOR || opcode == TEQ || opcode == TST || opcode == MOV);
}

int32_t executeAND(int32_t rn, uint32_t operand2 , int32_t rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = machinePtr->registers[rn] & operand2;
	return machinePtr->registers[rd];
}

int32_t executeEOR(int32_t rn, uint32_t operand2, int32_t rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = machinePtr->registers[rn] ^ operand2;
	return machinePtr->registers[rd];
}

int32_t executeSUB(int32_t rn, uint32_t operand2, int32_t rd, ARM_STATE *machinePtr, int32_t* carryptr) {
	if (machinePtr->registers[rn] >= operand2) {
		*carryptr = 1;
	}
	
	machinePtr->registers[rd] = machinePtr->registers[rn] - operand2;
	return machinePtr->registers[rd];
}

int32_t executeRSB(int32_t rn, uint32_t operand2 , int32_t rd, ARM_STATE *machinePtr, int32_t* carryptr) {
	if(machinePtr->registers[rn] <= operand2) {
		*carryptr = 1;
	}		
	
	machinePtr->registers[rd] = operand2 - machinePtr->registers[rn];
	return machinePtr->registers[rd];
}

int32_t executeADD(int32_t rn, uint32_t operand2 , int32_t rd, ARM_STATE *machinePtr, int32_t* carryptr) {
	if ((INT_MAX - operand2) < machinePtr->registers[rn]) {
                *carryptr = 1;
        }
	
	machinePtr->registers[rd] = machinePtr->registers[rn] + operand2;
	return machinePtr->registers[rd];
}

int32_t executeTST(int32_t rn, uint32_t operand2 , ARM_STATE *machinePtr) {
	return machinePtr->registers[rn] & operand2;
}

int32_t executeTEQ(int32_t rn, uint32_t operand2 , ARM_STATE *machinePtr) {
	return machinePtr->registers[rn] ^ operand2;
}

int32_t executeCMP(int32_t rn, uint32_t operand2 , ARM_STATE *machinePtr, int32_t* carryptr) {
        if (machinePtr->registers[rn] >= operand2) {
                *carryptr = 1;
        }
	
	return machinePtr->registers[rn] - operand2;
}

int32_t executeORR(int32_t rn, uint32_t operand2 , int32_t rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = machinePtr->registers[rn] | operand2;
	return machinePtr->registers[rd];
}

void executeMOV(uint32_t operand2 , int32_t rd, ARM_STATE *machinePtr) {
	machinePtr->registers[rd] = operand2;
}

//Shifts the value in register rm by a specified amount in a specified way (left, right, arithmetic etc)
int32_t shiftByConst(int32_t rm, int32_t shift, int32_t setFlags, ARM_STATE *ptr) {

	int32_t carryout;

	int32_t bit4 = shift & ONE_BIT_MASK;
	int32_t rs = (uint32_t) shift >> RS_SHIFT_DP;

	int32_t val = ptr->registers[rm];

	int32_t amt = bit4 == 1 ? rs & EIGHT_BIT_MASK : (uint32_t) shift >> 3;
	int32_t type = ((uint32_t) shift >> 1) & TWO_BIT_MASK;

	switch(type) {
		case LSL:
			carryout = (val >> (REG_SIZE - amt)) & ONE_BIT_MASK;
			val <<= amt;
			break;
		case LSR:
			carryout = (val >> amt) & ONE_BIT_MASK;
			val = (uint32_t) val >> amt;
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
