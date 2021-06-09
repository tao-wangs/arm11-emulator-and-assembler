#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int32_t assembleDataProcessing(char *mnemonic, char *srcreg, char *dstreg, char *operand2) {

  // All Data Processing Instructions should populate the Cond field with 1110 (which corresponds to the always condition.
  int32_t condCode = 0xE << CONDCODE_SHIFT;
  int32_t opcode; //Lookup function for symbol table not implemented yet
  int32_t filler = 0x0 << 26;
	
  int32_t rn;
  int32_t rd;
  int32_t setFlags;
  int32_t immOperand;
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

  switch(opcode) {
  case ORR:
  case ADD:
  case RSB:
  case SUB:
  case EOR:
  case AND:
    rn = stringToInt(srcreg) << 16;
    rd = stringToInt(dstreg) << 12;
    break;
  case CMP:
  case TEQ:
  case TST:
    rn = stringToInt(srcreg) << 16;
    
  
  return condCode | filler | immOperand | opcode | setFlags | rn | rd | operand2;
}
