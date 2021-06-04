#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include "binreader.h"
#include "../utility.h"

// returns int* of all instructions in the input bin file
uint32_t* readBin(char* fileName) {
  long int fsize = fSize(fileName);
  uint32_t *prog = malloc(sizeof(uint32_t) * fsize);
  FILE *fp;
  fp = fopen(fileName, "r");

  if(errno != 0){
    printf("Action returned the following errno: %i\n", errno);
    exit(errno);
  }

  fread(prog, INSTRUCTION_SIZE, (fsize/INSTRUCTION_SIZE), fp);
  fclose(fp);
  return prog;
}

//returns the size of a file in bytes
long int fSize(char* fileName) {
  FILE *fp = fopen(fileName, "r");

  if(errno != 0){
    printf("Action returned the following errno: %i\n", errno);
    exit(errno);
  }

  fseek(fp, 0, SEEK_END);
  long int size = ftell(fp);
  fclose(fp);
  return size;
}

char* binRep(uint32_t inst){
  char* rep = malloc(sizeof(char) * 32);
  uint32_t shifted;

  for(int32_t i = 0; i < UINT_SIZE * 8; i++){
    shifted = inst >> i;
    if (shifted & 1){
      rep[((UINT_SIZE * 8) - 1) - i] = '1';

    } else {
      rep[((UINT_SIZE * 8) - 1) - i] = '0';

    }
  }

  return rep;
}
