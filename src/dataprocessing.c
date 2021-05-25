#include <stdio.h>
#include <string.h>
#include "dataprocessing.h"


void dataProcessingInstruction(char *instruction) {
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
}

// Checks whether the immediate operand bit is set to 1 or not.
int immediateOperandBitIsSet(char immediateOperand) {
	return strcmp(immediateOperand, "1") == 0;
}

// Check whether the flags condition code is set.
int conditionCodeIsSet(char setConditionCode) {
	return strcmp(setConditionCode, "1") == 0;
}	

// See whether the opcode string is equal to that off any of the arithmetic operations, and return 1 if it matches any of them.
int operationIsArithmetic(char *opcode) {
	return !(strcmp(opcode, add) && 
		strcmp (opcode, sub) && 
		strcmp (opcSode, rsb) && 
		strcmp(opcode, cmp));
}

// See whether the opcode string is equal to that off any of the logical operations, and return 1 if it matches any of them.
int operationIsLogic(char *opcode) {
	return !(strcmp(opcode, and) && 
		strcmp(opcode, eor) && 
		strcmp(opcode, orr) && 
		strcmp(opcode, teq) && 
		strcmp(opcode, tst) && 
		strcmp(opcode, mov));
}

int executeAND(int rn, int operand2, int rd) {
	ARM_machine.registers[atoi(rd)] = ARM_machine.registers[atoi(rn)] & atoi(operand2);
	return ARM_machine.registers[atoi(rd)];
}

int executeEOR(int rn, int operand2, int rd) {
	ARM_machine.registers[atoi(rd)] = ARM_machine.registers[atoi(rn)] ^ atoi(operand2);
	return ARM_machine.registers[atoi(rd)];
}s

int executeSUB(int rn, int operand2, int rd) {
	ARM_machine.registers[atoi(rd)] = ARM_machine.registers[atoi(rn)] - atoi(operand2);
	return ARM_machine.registers[atoi(rd)];
}

int executeRSB(int rn, int operand2, int rd) {
	ARM_machine.registers[atoi(rd)] = atoi(operand2) - ARM_machine.registers[atoi(rn)];
	return ARM_machine.registers[atoi(rd)];
}

int executeADD(int rn, int operand2, int rd) {
	ARM_machine.registers[atoi(rd)] = ARM_machine.registers[atoi(rn)] + atoi(operand2);
	return ARM_machine.registers[atoi(rd)];
}
		
int executeTST(int rn, int operand2, int rd) {
	return ARM_machine.registers[atoi(rn)] & atoi(operand2);
}

int executeTEQ(int rn, int operand2, int rd) {
	return ARM_machine.registers[atoi(rn)] ^ atoi(operand2);
}

int executeCMP(int rn, int operand2, int rd) {
	return ARM_machine.registers[atoi(rn)] - atoi(operand2);	
}

int executeORR(int rn, int operand2, int rd) {
	return ARM_machine.registers[atoi(rn)] || atoi(operand2);
}

int executeMOV(int operand2, int rd) {
	ARM_machine.registers[atoi(rd)] = atoi(operand2);
}

int main(void) {

	char *instruction = "11110000100100000000000000000000";

	char *cond = malloc(4);
	char *immediateOperand = malloc(1);
	char *opcode = malloc(4);
	char *setConditionCode = malloc(1);
	char *rn = malloc(4);
	char *rd = malloc(4);
	char *operand2 = malloc(12);
	
	strncpy(cond, instruction, 4);
	printf("cond: %s\n", cond);

	strncpy(immediateOperand, instruction+6, 1);
	printf("imm: %s\n", immediateOperand);
	strncpy(opcode, instruction+7, 4);
	printf("opcode: %s\n", opcode);
	
	strncpy(setConditionCode, instruction+11, 1);
	printf("setcond: %s\n", setConditionCode);
	strncpy(rn, instruction+12, 4);
	printf("rn: %s\n", rn);
	
	strncpy(rd, instruction+16, 4);
	printf("rd: %s\n", rd);
	
	strncpy(operand2, instruction+20, 12);
	printf("op2: %s\n", operand2);	

	printf("\n");

	printf("immset? %i\n", immediateOperandBitIsSet(immediateOperand));
	printf("condset? %i\n", conditionCodeIsSet(setConditionCode));
	printf("arith? %i\n", operationIsArithmetic(opcode));
	printf("logic? %i\n", operationIsLogic(opcode));

	return 0;
}
