#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "hash.h"
#include "utility.h"
#include "assdataprocessing.h"

int32_t assembleDataProcessing(char *instrString, hashTable *table) {
	
	char *saveptr;
	char *mnemonic;
	char *dstreg;
	char *srcreg;
	char *op2;

	mnemonic = strtok_r(instrString, " ", &saveptr);
	
	int32_t condCode = AL << CONDCODE_SHIFT;
	int32_t opcode = ((int32_t) lookupVal(table, mnemonic)) << OPCODE_SHIFT;
	int32_t filler = 0x0 << FILLER_SHIFT;
	
	int32_t rn;
        int32_t rd;
        int32_t setFlags;
        int32_t immOperand = 1 << IMM_OPERAND_SHIFT;
        int32_t operand2;
	
	// If it is not one of the testing instructions then we should set the S bit to 1, for all other instructions you should set the S bit to 0.
        if (!(strcmp(mnemonic, "tst") && strcmp(mnemonic, "teq") && strcmp(mnemonic, "cmp"))) {
		dstreg = NULL;
		srcreg = strtok_r(NULL, " ", &saveptr);	
        	setFlags = 1 << SET_FLAGS_SHIFT;
        } else if (!strcmp(mnemonic, "mov")) {
		dstreg = strtok_r(NULL, " ", &saveptr);
		srcreg = NULL;
		setFlags = 0 << SET_FLAGS_SHIFT;
	} else {
		dstreg = strtok_r(NULL, " ", &saveptr);
		srcreg = strtok_r(NULL, " ", &saveptr);
		setFlags = 0 << SET_FLAGS_SHIFT;
	}
	
	op2 = strtok_r(NULL, " ", &saveptr);
	
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

	while (immOperand % 2 == 0) { //keep shifting left until until LSB is 1
		immOperand >>= 1;
		shift_amt++;
	}

	if (shift_amt % 2 == 1) { //shift_amt is halved at the end so it must be even. If odd, shift right once to decrement shift_amt and make it even
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

int main(int argc, char **argv) {

	assert (argc == 2);
	
	hashTable *table = createHashTable(10);

	char *operands[10] = {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov"};
	uint64_t opcodes[10] = {0, 1, 2, 3, 4, 8, 9, 10, 12, 13};
	
	addHashList(table, operands, opcodes);

	//int32_t res = assembleDataProcessing(argv[1], table);
	int32_t res = lookupVal(table, argv[1]);

	printf("%u\n", res);

	return EXIT_SUCCESS;

}
