#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "assdataprocessing.h"

uint32_t assembleSpecialInstruction(char *instrString, hashTable *table) {
	
	char **tokens;
	char *saveptr;

	char *mnemonic = strtok_r(instrString, " ", &saveptr);

	if (!strcmp(mnemonic, "lsl")) {
		tokens = tok(saveptr, 2);
		int32_t condCode = AL << CONDCODE_SHIFT;
		int32_t filler = 0x0 << FILLER_SHIFT;
		int32_t immOperand = 0 << IMM_OPERAND_SHIFT;
		int32_t opcode = lookupVal(table, "mov") << OPCODE_SHIFT;
    		int32_t rn = stringToInt(tokens[0]);
    		int32_t setFlags = 0 << SET_FLAGS_SHIFT;
		int32_t shiftValue = stringToInt(tokens[1]);

		if (shiftValue > FIVE_BIT_MAX_INT) {
			perror("Cannot be shifted by this much");
			exit(EXIT_FAILURE);
		}
	
		return condCode | filler | immOperand | opcode | setFlags | (rn << RD_SHIFT) | (shiftValue << SHIFT_VALUE_SHIFT) | rn;
	}
	
	if (!strcmp(mnemonic, "andeq")) {
		tokens = tok(saveptr, 3);
		if (!strcmp(tokens[0], "r0") && !strcmp(tokens[1], "r0") && !strcmp(tokens[2], "r0")) {
			return 0x00000000;
		}
	}

	return 0;
}

uint32_t assembleDataProcessing(char *instrString, hashTable *table) {
	
	char *saveptr;
	char **tokens;

	char *mnemonic;
	char *dstreg;
	char *srcreg;
	char *op2;
	
        int32_t setFlags;
	
	mnemonic = strtok_r(instrString, " ", &saveptr);

	// If it is not one of the testing instructions then we should set the S bit to 1, for all other instructions you should set the S bit to 0.
        if (!(strcmp(mnemonic, "tst") && strcmp(mnemonic, "teq") && strcmp(mnemonic, "cmp"))) {
		tokens = tok(saveptr, 2);
		dstreg = NULL;
		srcreg = tokens[0];
		op2 = tokens[1];
        	setFlags = 1 << SET_FLAGS_SHIFT;
        } else if (!strcmp(mnemonic, "mov")) {
		tokens = tok(saveptr, 2);
		dstreg = tokens[0];
		srcreg = NULL;
		op2 = tokens[1];
		setFlags = 0 << SET_FLAGS_SHIFT;
	} else {
		tokens = tok(saveptr, 3);
		dstreg = tokens[0];
		srcreg = tokens[1];
		op2 = tokens[2];
		setFlags = 0 << SET_FLAGS_SHIFT;
	}
	
	int32_t condCode = AL << CONDCODE_SHIFT;
	int32_t filler = 0x0 << FILLER_SHIFT;
	int32_t immOperand = 1 << IMM_OPERAND_SHIFT;
	int32_t opcode = lookupVal(table, mnemonic) << OPCODE_SHIFT;
	int32_t rn = ((srcreg == NULL) ? 0x0 : stringToInt(srcreg)) << RN_SHIFT;
	int32_t rd = ((dstreg == NULL) ? 0x0 : stringToInt(dstreg)) << RD_SHIFT;
	int32_t operand2 = generate8BitImmediate(op2);

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

	while (immOperand % 2 == 0) { //keep shifting left until our immediate operand until LSB is 1
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

/*
int main(int argc, char **argv) {

	hashTable *table = createHashTable(10);

	char *operands[10] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov"};
	uint64_t opcodes[10] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13};

	addHashList(table, operands, opcodes);

	int32_t res = assembleSpecialInstruction(argv[1], table);
	
	printf("%u\n", res);
	return EXIT_SUCCESS;
}
*/
