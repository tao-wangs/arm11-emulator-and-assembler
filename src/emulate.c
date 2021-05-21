#include <stdlib.h>
#include <stdio.h>

#define INSTRUCTION_SIZE 4

int* readBin(char* fileName);
long int fSize(char* fileName);

int main(int argc, char **argv) {
  readBin(argv[1]); // reads the file input
  return 0;
}

// returns int* of all instructions in the input bin file
int* readBin(char* fileName) {
  long int fsize = fSize(fileName); 
  int buffer[fsize];
  FILE *fp;
  fp = fopen(fileName,"r");
  fread(buffer, INSTRUCTION_SIZE, (fsize/INSTRUCTION_SIZE), fp);
  fclose(fp);
  return buffer;
}

//returns the size of a file in bytes
long int fSize(char* fileName){
  FILE *fp = fopen(fileName, "r");
  fseek(fp, 0, SEEK_END);
  long int size = ftell(fp);
  fclose(fp);
  return size;
}
