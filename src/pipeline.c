#include <stdio.h>
#include <stdlib.h>

#include "binreader.h"
#include "decode.h"
#include "arm_state.h"
#include "multiply.h"

#define PC 15
#define INSTRUCTION_SIZE 4

typedef struct {
  int fetchedInstr;
  int decodedInstr;
} PIPELINE_STATE;

void loadInstrToMem(ARM_STATE *state, char *progName){
  long int fsize = fSize(progName);
  unsigned int *loadedBin = readBin(progName);
  for(int i = 0; i < fsize; i++){
    state->memory[i] = loadedBin[i];
  }
}

void pipeline(ARM_STATE *state){
  enum INSTRUCTION_TYPE type;
 
  PIPELINE_STATE pipe;
  PIPELINE_STATE *pipePtr = &pipe;

  pipePtr->fetchedInstr = 0;
  pipePtr->decodedInstr = 0; 

  do {
    if(pipePtr->decodedInstr == 0){
      if(pipePtr->fetchedInstr == 0){
	pipePtr->fetchedInstr = state->memory[state->registers[PC]/INSTRUCTION_SIZE];
      }
      type = decode(pipePtr->fetchedInstr);
      pipePtr->decodedInstr = pipePtr->fetchedInstr;
      
      state->registers[PC] += 4;
      pipePtr->fetchedInstr = state->memory[state->registers[PC]/INSTRUCTION_SIZE];
      
      
    } else {
      
      //insert switch case here for each type of instruction to execute
      switch (type){
        case Multiply:
	  decodeMultiply(pipePtr->decodedInstr, state);
	case Branch:
	  pipePtr->decodedInstr = 0;
          goto fetch;	  
	default:
	  printf("Instruction not recognised");
	  exit(1);
      }

      type = decode(pipePtr->fetchedInstr);
      pipePtr->decodedInstr = pipePtr->fetchedInstr;
      
      fetch:
        state->registers[PC] += 4;
        pipePtr->fetchedInstr = state->memory[state->registers[PC]/INSTRUCTION_SIZE];

    }
  } while(pipePtr->fetchedInstr != 0);
}

