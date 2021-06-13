#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "tokeniser.h"

char** tok(char* instr, uint32_t ops){
    uint32_t i = 0;
    char *savePtr;
    char **tokens = malloc(sizeof(char*) * ops);
    char *token = " ";

    while(token != NULL && i < ops){
        if(i == 0){
	    token = strtok_r(instr, " :,\n", &savePtr);
	} else {
	    token = strtok_r(NULL, " :,\n", &savePtr);	
	}

        tokens[i] = token;
        i++;	
    }

    return tokens;
}

void freeTok(char** tokens){
    free(tokens);
}
