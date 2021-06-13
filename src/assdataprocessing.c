#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "hash.h"
#include "tokeniser.c"
#include "utility.h"
#include "assdataprocessing.h"

int32_t assembleSpecialInstruction(char *instrString, hashTable *table) {
	
	char **tokens;
	char *saveptr;

	char *mnemonic = strtok_r(instrString, " ", &saveptr);
	
	int32_t condCode;
	int32_t 
	int32_t setFlags;


	if (!strcmp(mnemonic, "lsl")) {
		int32_t condCode = AL << CONDCODE_SHIFT;
      		int32_t opcode = lookupVal(table, "mov");
    		int32_t rn = ((srcreg == NULL) ? 0x0 : stringToInt(srcreg));
    		int32_t setFlags = 0 << SET_FLAGS_SHIFT;
    		int32_t filler = 0x0 << FILLER_SHIFT;
    		int32_t immOperand = 0 << IMM_OPERAND_SHIFT;
    		int32_t shiftValue = generate8BitImmediate(op2);

  		return condCode | filler | immOperand | (opCode << 21) | setFlags | (rn << RN_SHIFT) | (shiftValue << 7) | rn;
	}
	
	if (!(strcmp(mnemonic, "andeq") && strcmp(dstreg, "r0") && strcmp(srcreg, "r0") && strcmp(op2, "r0"))) {
		return 0x00000000;
  	}
}

int32_t assembleDataProcessing(char *mnemonic, char *dstreg, char *srcreg, char *op2, hashTable *table) {

	int32_t condCode = AL << CONDCODE_SHIFT;
	int32_t opcode = lookupVal(table, mnemonic);
	int32_t filler = 0x0 << FILLER_SHIFT;
	
  	int32_t rn;
  	int32_t rd;
  	int32_t setFlags;
  	int32_t immOperand = 1 << IMM_OPERAND_SHIFT;
  	int32_t operand2;

  	// If it is not one of the testing instructions then we should set the S bit to one, for all other instructions you should set the S bit to -.
  	if (!(strcmp(mnemonic, "tst") && strcmp(mnemonic, "teq") && strcmp(mnemonic, "cmp"))) {
  	  setFlags = 1 << SET_FLAGS_SHIFT;
  	} else {
  	  setFlags = 0 << SET_FLAGS_SHIFT;
  	}
  
    	rn = ((srcreg == NULL) ? 0x0 : stringToInt(srcreg)) << RN_SHIFT; 
    	rd = ((dstreg == NULL) ? 0x0 : stringToInt(dstreg)) << RD_SHIFT;

	operand2 = generate8BitImmediate(op2);

	return condCode | filler | immOperand | opcode | setFlags | rn | rd | operand2;
}
int32_t generate8BitImmediate(char *operand2) {
	int32_t operand2_as_int = stringToInt(operand2);
  	if (operand2_as_int > ONE_BYTE_MAX_INT) {
    		return undoRotation(operand2_as_int);
  	}
  	return operand2_as_int;
}

int32_t undoRotation(int32_t immOperand) {

	int32_t shift_amt = 0;

	while (immOperand % 2 == 0) { //keep shifting left until our immediate operand is odd i.e. until LSB is 1
		immOperand >>= 1;
		shift_amt++;
	}

	if (shift_amt % 2 == 1) { //shift_amt is halved the end so it must be even. if odd, shift right once to decrease shift_amt and make it even
		immOperand <<= 1;
		shift_amt--;
	}

	if (immOperand > ONE_BYTE_MAX_INT) {
		perror("This operand cannot be represented using 8 bits.");
		exit(EXIT_FAILURE);
	}

	int32_t rotate_amt = (INSTRUCTION_BITS - shift_amt) / 2;
	if (rotate_amt > ONE_NIBBLE_MAX_INT) {
		perror("This operand cannot be represented using 8 bits.");
		exit(EXIT_FAILURE);
	}

	return rotate_amt << ROTATE_AMT_SHIFT | immOperand;
}	
