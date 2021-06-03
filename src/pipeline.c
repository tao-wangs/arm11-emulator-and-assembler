#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "binreader.h"
#include "decode.h"
#include "arm_state.h"
#include "multiply.h"
#include "singledatatransfer.h"
#include "branch.h"
#include "dataprocessing.h"
#include "utility.h"
#include "pipeline.h"

void loadInstrToMem(ARM_STATE *state, char *progName){
  long int fsize = fSize(progName);
  uint32_t *loadedBin = readBin(progName);
  for(int32_t i = 0; i < fsize; i++){
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
	  break;
	case Branch:
	  if (executeBranch(pipePtr->decodedInstr, state)){
	    pipePtr->decodedInstr = 0;
	    goto fetch; //skip decode
	  }
	  break;
	case DataProcessing:
	  dataProcessingInstruction(pipePtr->decodedInstr, state);
	  break;
	case SingleDataTransfer:
	  decodeSDT(pipePtr->decodedInstr, state);
	  break;
	case Halt:
	  goto stop;
	default:
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
}

