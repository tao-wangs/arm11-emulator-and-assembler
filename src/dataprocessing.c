#include <stdio.h>
#include <string.h>

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

int operationIsArithmetic(char *opcode) {

}

int operationIsLogic(char *opcode) {

}
