#include <stdlib.h>
#include <stdio.h>
#include "binreader.h"
#include "arm_state.h"

int main(int argc, char **argv) {
  unsigned int *prog = readBin(argv[1]); // reads the file input
  printf("%x\n", prog[1]);
  char* bin = binRep(prog[1]);
  for(int i = 0; i<32; i++){
    printf("%c", bin[i]);
  }
  printf("\n");

  ARM_STATE ARM_machine;
  ARM_STATE *machinePtr = &ARM_machine;

  initialise(machinePtr);

  readBin(argv[1]); // reads the file input

  //fetch decode execute loop here until termination

  terminate(machinePtr);
  return 0;
}

