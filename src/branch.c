#include "multiply.h"
#include <stdio.h>

void executeBranch(unsigned int instr, ARM_STATE *state){
  int condMask = 0xF0000000;
  int offMask = 0x00FFFFFF;
  int signedMask = 0x00100000;

  int checkSign = (instr & signedMask) >> 23;
  int offset = (instr & offMask);
  unsigned int cond = (instr & condMask) >> 28;

  if (conditionMet(cond, state)){
    offset <<= 2;
    if (checkSign){
      offset |= 0xFC000000; //add 6 1s for sign extension 
    }

    state->registers[PC] += offset;
  }
  
}
