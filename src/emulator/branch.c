
#include "branch.h"

int32_t executeBranch(uint32_t instr, ARM_STATE *state) {

  int32_t checkSign = (instr >> SIGN_SHIFT) & ONE_BIT_MASK;
  int32_t offset = (instr & OFFSET_MASK_BRANCH);
  uint32_t cond = (instr >> CONDCODE_SHIFT) & FOUR_BIT_MASK;
  
  if (conditionMet(cond, state)){
    offset <<= 2;
    if (checkSign){
      offset |= SIGN_EXTEND_MASK; //sign extend to 32 bits
    }

    state->registers[PC] += offset;
    return 1;
  } else {
    return 0; //branch did not execute
  }

}

