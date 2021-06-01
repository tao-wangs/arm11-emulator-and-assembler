#include <stdio.h>

#include "branch.h"
#include "decode.h"

int executeBranch(unsigned int instr, ARM_STATE *state) {

  int checkSign = (instr & SIGNED_MASK) >> 23;
  int offset = (instr & OFF_MASK);
  unsigned int cond = (instr & COND_MASK) >> 28;
  
  if (conditionMet(cond, state)){
    offset <<= 2;
    if (checkSign){
      offset |= 0xFC000000; //add 6 1's for sign extension
    }

    state->registers[PC] += offset;
    return 1;
  } else {
    return 0; //branch did not execute
  }

}
