#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "hash.h"
#include "utility.h"
#include "assdataprocessing.h"

int32_t assembleDataProcessing(char *mnemonic, char *dstreg, char *srcreg, char *op2, hashTable *table) {

	int32_t condCode = 0xE << CONDCODE_SHIFT;
	int32_t opcode = lookupVal(table, mnemonic);
	int32_t filler = 0x0 << 26;
	
  int32_t rn;
  int32_t rd;
  int32_t setFlags = 0 << 20;
  int32_t immOperand = 1 << 25;
  int32_t operand2;

  // If it is not one of the testing instructions then we should set the S bit to one, for all other instructions you should set the S bit to -.
  if (!(strcmp(mnemonic, "tst") && strcmp(mnemonic, "teq") && strcmp(mnemonic, "cmp"))) {
    setFlags = 1 << 20;
  } else {
    setFlags = 0 << 20;
  }
  
    rn = ((srcreg == NULL) ? 0x0 : stringToInt(srcreg)) << 16; 
    rd = ((dstreg == NULL) ? 0x0 : stringToInt(dstreg)) << 12;

	rn = ((srcreg == NULL) ? 0x0 : stringToInt(srcreg)) << 16; 
	rd = ((dstreg == NULL) ? 0x0 : stringToInt(dstreg)) << 12;

	operand2 = generate8BitImmediate(op2);

int32_t undoRotation(int32_t operand2_as_int) {
  int32_t shift_amount = 0;
  // So while operand2 is even, i.e. divisible by 2 we shift it to the right, i.e. we divide it by 2, and we increment shift and keep on doing so whilst even.
  // Binary numbers are always even unless the right most (least significant) bit is 1, as all the others represent even numbers (2,4,8 etc).
  while(operand2_as_int % 2 == 0) {
    operand2_as_int = operand2_as_int >> 1;
    shift_amount++;
  } if(operand2_as_int >= max_8bit_value) {
    perror("The immediate value of operand2 cannot be represented using 8 bits");
    exit(EXIT_FAILURE);
  } // If our shift amount is odd then we reverse it, as we can only rotate to the right by an even number of positions.
  if(shift_amount % 2 == 1) {
    operand2_as_int = operand2_as_int << 1;
  }
  int32_t rotate = (MAX_AMOUNT_CAN_ROTATE - shift_amount) / 2;
  if(rotate > ONE_NIBBLE_MAX_INT) {
    perror("The immediate value of operand2 cannot be represented using 8 bits");
    exit(EXIT_FAILURE);
  }
  return (rotate << 8) | operand2_as_int;
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

	return rotate_amt << 8 | immOperand;
}	
