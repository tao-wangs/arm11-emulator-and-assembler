#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define max_8bit_value 0b11111111
#define MAX_AMOUNT_CAN_ROTATE 16

int32_t assembleDataProcessing(char *mnemonic, char *srcreg, char *dstreg, char *operand2) {

  // All Data Processing Instructions should populate the Cond field with 1110 (which corresponds to the always condition).
  int32_t condCode = 0xE << CONDCODE_SHIFT;
  int32_t opcode; //Lookup function for symbol table not implemented yet
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
  
  rn = stringToInt(srcreg) << 16;
  rd = stringToInt(dstreg) << 12;
  
  
  /* some code */
  if(operand2 > max_8bit_value) {
    operand2 =  generate8BitImmediate(operand2);
  }
  return condCode | filler | immOperand | opcode | setFlags | rn | rd | operand2;
}


int32_t generate8BitImmediate(char *operand2) {
  int32_t operand2_as_int = atoi(operand2);
  if (operand2_as_int > max_8bit_value) {
    return undoRotation(operand2_as_int);
  }
  return operand2_as_int;
}

int32_t undoRotation(int32_t operand2_as_int) {
  int32_t shift_amount = 0;
  // So while operand2 is even, i.e. divisible by 2 we shift it to the right, i.e. we divide it by 2, and we increment shift and keep on doing so whilst even.
  // Binary numbers are always even unless the right most (least significant) bit is 1, as all the others represent even numbers (2,4,8 etc).
  while(operand2_as_int % 2 == 0) {
    operand2_as_int = operand2_as_int >> 1;
    shift_amount++;
  } if(operand2_as_int > max_8bit_value) {
    printf("The immediate value of operand2 cannot be represented using 8 bits");
    return(EXIT_FAILURE);
  } // If our shift amount is odd then we reverse it, as we can only rotate to the right by an even number of positions.
  if(shift_amount % 2 == 1) {
    operand2_as_int = operand2_as_int << 1;
  }
  int32_t rotate = MAX_AMOUNT_CAN_ROTATE - shift_amount / 2;
  return (rotate << 8) + operand2_as_int;
}
