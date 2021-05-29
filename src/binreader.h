#ifndef BINREADER_H
#define BINREADER_H

#define INSTRUCTION_SIZE 4
#define UINT_SIZE sizeof(unsigned int)

unsigned int* readBin(char* fileName);
long int fSize(char* fileName);
char* binRep(unsigned int inst);

#endif
