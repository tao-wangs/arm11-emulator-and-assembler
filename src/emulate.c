#include <stdlib.h>
#include <stdio.h>
#include "binreader.h"
#include "arm_state.h"

int main(int argc, char **argv) {
  //unsigned int *prog = readBin(argv[1]); // reads the file input
  
  ARM_STATE ARM_machine;
  ARM_STATE *machinePtr = &ARM_machine;

  initialise(machinePtr);

  //fetch decode execute loop here until termination

  terminate(machinePtr);
  return 0;
}

