#ifndef PIPELINE_H
#define PIPELINE_H

#include "binreader.h"
#include "decode.h"
#include "multiply.h"
#include "singledatatransfer.h"
#include "branch.h"
#include "dataprocessing.h"
#include "../utility.h"

typedef struct {
  int32_t fetchedInstr;
  int32_t decodedInstr;
} PIPELINE_STATE;

void loadInstrToMem(ARM_STATE *state, char *progName);
void pipeline(ARM_STATE *state);

#endif
