#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "hash.h"
#include "firstpass.h"

#define MAX_LINE_SIZE 511

uint32_t firstPass(hashTable *labels, char *filename){ //returns final address in the program for SDT
    FILE *fp = fopen(filename, "r");
    //char *labelList[getOriginalSize(labels)];
    //uint64_t valList[getOriginalSize(labels)];
    char buffer[MAX_LINE_SIZE];
    uint32_t addr = 0;

    fgets(buffer, MAX_LINE_SIZE, fp);
    while(!feof(fp)){
        if(isLabel(buffer)){
	    char *copiedBuffer = malloc(sizeof(char) * MAX_LINE_SIZE);
	    strcpy(copiedBuffer, buffer);
	    addHashItem(labels, copiedBuffer, addr);
	    addr -= 4;
	}
  if (!isspace(buffer[0])) {
    addr += 4;
  }
	
  fgets(buffer, MAX_LINE_SIZE, fp);
    }
    //addHashList(labels, labelList, valList);
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
	    //printf("LABEL %s %c\n", line, line[i]);
	    line[i] = '\0'; 
	    return true; //label detected
        } else if(line[i] == '\n'){
	    break;
	}
   }
   return false; //end of line reached, no label
}
