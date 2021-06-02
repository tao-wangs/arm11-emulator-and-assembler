#ifndef PIPELINE_H
#define PIPELINE_H

#define eof 26
#define PC 15
#define INSTRUCTION_SIZE 4

typedef struct {
  int32_t fetchedInstr;
  int32_t decodedInstr;
} PIPELINE_STATE;

void loadInstrToMem(ARM_STATE *state, char *progName);

void pipeline(ARM_STATE *state);

#endif
