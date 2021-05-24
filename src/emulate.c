#include <stdlib.h>
#include <stdio.h>
#include "binreader.h"

int main(int argc, char **argv) {
  unsigned int *prog = readBin(argv[1]); // reads the file input
  printf("%x\n", prog[1]);
  char* bin = binRep(prog[1]);
  for(int i = 0; i<32; i++){
    printf("%c", bin[i]);
  }
  printf("\n");
  return 0;
}

