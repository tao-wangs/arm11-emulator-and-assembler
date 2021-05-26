#include <stdlib.h>
#include <stdio.h>

#define INSTRUCTION_SIZE 4
#define UINT_SIZE sizeof(unsigned int)

unsigned int* readBin(char* fileName);
long int fSize(char* fileName);
char* binRep(unsigned int inst);

// returns int* of all instructions in the input bin file
unsigned int* readBin(char* fileName) {
  long int fsize = fSize(fileName); 
  unsigned int *prog = malloc(sizeof(unsigned int) * fsize);
  FILE *fp;
  fp = fopen(fileName,"r");
  fread(prog, INSTRUCTION_SIZE, (fsize/INSTRUCTION_SIZE), fp);
  fclose(fp);
  return prog;
}

//returns the size of a file in bytes
long int fSize(char* fileName){
  FILE *fp = fopen(fileName, "r");
  fseek(fp, 0, SEEK_END);
  long int size = ftell(fp);
  fclose(fp);
  return size;
}

char* binRep(unsigned int inst){
  char* rep = malloc(sizeof(char) * 32);
  unsigned int shifted;  

  for(int i = 0; i < UINT_SIZE * 8; i++){
    shifted = inst >> i;
    if (shifted & 1){
      rep[((UINT_SIZE*8)-1)-i] = '1';
      
    } else {
      rep[((UINT_SIZE*8)-1)-i] = '0';
      
    }
  }

  return rep;    
}
