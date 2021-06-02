#include <stdio.h>
#include <stdint.h>

#include "branch.h"
#include "decode.h"

int32_t executeBranch(uint32_t instr, ARM_STATE *state) {

  int32_t checkSign = (instr & SIGNED_MASK) >> 23;
  int32_t offset = (instr & OFF_MASK);
  uint32_t cond = (instr & COND_MASK) >> 28;
  
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

