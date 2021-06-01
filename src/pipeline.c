#include <stdio.h>
#include <stdlib.h>

#include "binreader.h"
#include "decode.h"
#include "arm_state.h"
#include "multiply.h"
#include "singledatatransfer.h"
#include "branch.h"
#include "dataprocessing.h"

#define eof 26
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
    state->memory[i] = toLittleEndian(loadedBin[i]);
  }
}

void pipeline(ARM_STATE *state){
  INSTRUCTION_TYPE type;

  PIPELINE_STATE pipe;
  PIPELINE_STATE *pipePtr = &pipe;

  pipePtr->fetchedInstr = 0;
  pipePtr->decodedInstr = 0;

  while (type!=Halt) {
    if(pipePtr->decodedInstr == 0){
      if(pipePtr->fetchedInstr == 0){
	pipePtr->fetchedInstr = toLittleEndian(state->memory[state->registers[PC]/INSTRUCTION_SIZE]);
	state->registers[PC] += 4;
      }
      type = decode(pipePtr->fetchedInstr);
      pipePtr->decodedInstr = pipePtr->fetchedInstr;

      pipePtr->fetchedInstr = toLittleEndian(state->memory[state->registers[PC]/INSTRUCTION_SIZE]);
      state->registers[PC] += 4;

    } else {

      //insert switch case here for each type of instruction to execute
      switch (type){
        case Multiply:
	  decodeMultiply(pipePtr->decodedInstr, state);
	  //printf("Multiply instruction executed\n");
	  break;
	case Branch:
	  if (executeBranch(pipePtr->decodedInstr, state)){
	    pipePtr->decodedInstr = 0;
	    goto fetch; //skip decode
	  }
	  
	  break;
	case DataProcessing:
	  dataProcessingInstruction(pipePtr->decodedInstr, state);
	  //printf("Data processing instruction executed\n");
	  break;
	case SingleDataTransfer:
	  decodeSDT(pipePtr->decodedInstr, state);
	  //printf("SDT instruction executed\n");
	  break;
	case Halt:
	  //printf("Halt instruction detected...\n");
	  goto stop;
	default:
	  //printf("Instruction not recognised, stopping execution...");
	  goto stop;
      }

      type = decode(pipePtr->fetchedInstr);
      pipePtr->decodedInstr = pipePtr->fetchedInstr;

      fetch:
      pipePtr->fetchedInstr = toLittleEndian(state->memory[state->registers[PC]/INSTRUCTION_SIZE]);
      state->registers[PC] += 4;

    }
  }

  stop:
  return;
  //printf("pipeline stopped\n");
}

