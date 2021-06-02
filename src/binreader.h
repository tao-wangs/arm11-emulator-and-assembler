#ifndef BINREADER_H
#define BINREADER_H

#define INSTRUCTION_SIZE 4
#define UINT_SIZE sizeof(unsigned int)

uint32_t* readBin(char* fileName);
long int fSize(char* fileName);
char* binRep(uint32_t inst);

#endif
