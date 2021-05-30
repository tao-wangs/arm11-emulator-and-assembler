#include <stdlib.h>
#include <stdio.h>

#include "arm_state.h"
#include "pipeline.h"

int main(int argc, char **argv) {
  if(argc != 2){
    printf("Invalid number of arguments provided! Terminating...\n");
    exit(1);
  }


  ARM_STATE ARM_machine;
  ARM_STATE *machinePtr = &ARM_machine;

  initialise(machinePtr);

  //fetch decode execute loop here until termination

  loadInstrToMem(machinePtr, argv[1]);
  pipeline(machinePtr);
  terminate(machinePtr);
  return 0;
}

