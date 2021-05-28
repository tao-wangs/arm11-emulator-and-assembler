#include <stdio.h>
#include <stdlib.h>

#include "binreader.h"
#include "decode.h"
#include "arm_state.h"
#include "multiply.h"
#include "singledatatransfer.h"
#include "branch.h"

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
  INSTRUCTION_TYPE type;
 
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
	  printf("Multiply instruction executed\n");
	case Branch:
	  executeBranch(pipePtr->decodedInstr, state);
	  printf("Branch instruction executed\n");
	  pipePtr->decodedInstr = 0;
          goto fetch; // skip decode
	case DataProcessing:
	  printf("Instruction type not implemented yet, stopping pipeline execution...\n");
	  goto stop;
	case SingleDataTransfer:
	  decodeSDT(pipePtr->decodedInstr, state);
	  printf("SDT instruction executed\n");
	default:
	  printf("Instruction not recognised, stopping pipeline execution...\n");
	  goto stop;
      }

      type = decode(pipePtr->fetchedInstr);
      pipePtr->decodedInstr = pipePtr->fetchedInstr;
      
      fetch:
      state->registers[PC] += 4;
      pipePtr->fetchedInstr = state->memory[state->registers[PC]/INSTRUCTION_SIZE];

    }
  } while(pipePtr->fetchedInstr != 0);
  
  stop:
  printf("pipeline stopped\n");
}

