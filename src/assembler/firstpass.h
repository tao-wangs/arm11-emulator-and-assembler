#ifndef __FIRSTPASS_H__
#define __FIRSTPASS_H__

#include "hash.h"

#define MAX_LINE_SIZE 511

uint32_t firstPass(hashTable *labels, char *filename);
long int fSize(char *filename);
bool isLabel(char *line);

#endif
