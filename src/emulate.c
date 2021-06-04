#include <stdlib.h>
#include <stdio.h>

#include "emulator/arm_state.h"
#include "emulator/pipeline.h"

int main(int argc, char **argv) {
  if(argc != 2){
    printf("Invalid number of arguments provided! Terminating...\n");
    exit(1);
  }

  ARM_STATE ARM_machine;
  ARM_STATE *machinePtr = &ARM_machine;

  initialise(machinePtr);

  loadInstrToMem(machinePtr, argv[1]);
  
  pipeline(machinePtr);
  
  terminate(machinePtr);
  
  return 0;
}

