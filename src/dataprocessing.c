#include <stdio.h>
#include <string.h>

#define AND "0000"
#define EOR "0001"
#define SUB "0010"
#define RSB "0011"
#define ADD "0100"
#define TST "1000"
#define TEQ "1001"
#define CMP "1010"
#define ORR "1100"
#define MOV "1101"

void dataProcessingInstruction(char *instruction) {
	char cond[4];
	char immediateOperand;
	char opcode[4];
	char setConditionCode;
	char rn[4];
	char rd[4];
	char operand2[12];
	
	strncpy(cond, instruction, 4);
	cond[4] = '\0';
	
	strncpy(immediateOperand, instruction+6, 1);
	strncpy(opcode, instruction+7, 4);
	
	strncpy(setConditionCode, instruction+11, 1);
	strncpy(rn, instruction+12, 4);
	rn[4] = '\0';
	
	strncpy(rd, instruction+16, 4);
	rd[4] = '\0';
	
	strncpy(operand2, instruction+20, 12);
	operand2[12] = '\0';
}

// Checks whether the immediate operand bit is set to 1 or not.
int immediateOperandBitIsSet(char immediateOperand) {
	return immediateOperand == '1';
}

// Check whether the flags condition code is set.
int conditionCodeIsSet(char setConditionCode) {
	return setConditionCode == '1';
}	

// See whether the opcode string is equal to that off any of the arithmetic operations, and return 1 if it matches any of them.
int operationIsArithmetic(char *opcode) {
	return ((strcmp(opcode, ADD) == 0) || 
		(strcmp(opcode, SUB) == 0) || 
		(strcmp(opcode, RSB) == 0) || 
		(strcmp(opcode, CMP) == 0))
}

// See whether the opcode string is equal to that off any of the logical operations, and return 1 if it matches any of them.
int operationIsLogic(char *opcode) {
	return ((strcmp(opcode, AND) == 0) || 
		(strcmp(opcode, EOR) == 0) || 
		(strcmp(opcode, ORR) == 0) || 
		(strcmp(opcode, TEQ) == 0) || 
		(strcmp(opcode, TST) == 0) || 
		(strcmp(opcode, CMP) == 0));
}
