#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include "hash.h"

#include "firstpass.h"

#define MAX_LINE_SIZE 511

int firstPass(hashTable *labels, char *filename){ //returns final address in the program for SDT
    FILE *fp = fopen(filename, "r");
    char *labelList[getOriginalSize(labels)];
    uint64_t valList[getOriginalSize(labels)];
    char buffer[MAX_LINE_SIZE];
    int addr = 0;
    int i = 0;

    while(!feof(fp)){
        fgets(buffer, MAX_LINE_SIZE, fp);
        if(isLabel(buffer)){
	    labelList[i] = buffer;
	    valList[i] = addr;
	    i += 1;    
	}
	addr += 4;
    }
    addHashList(labels, labelList, valList);
    fclose(fp);
    return addr;
}

long int fSize(char* filename) {
  FILE *fp = fopen(filename, "r");

  if(errno != 0){
    printf("Action returned the following errno: %i\n", errno);
    exit(errno);
  }

  fseek(fp, 0, SEEK_END);
  long int size = ftell(fp);
  fclose(fp);
  return size;
}

bool isLabel(char *line){
   for(int i = 0; i < MAX_LINE_SIZE; i++){
        if(line[i] == ':'){
	     line[i] = '\0'; 
	     return true; //label detected
        }
   }
   return false; //end of line reached, no label
}
