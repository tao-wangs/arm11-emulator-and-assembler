#ifndef ASSEMBLEMULTIPLY_H
#define ASSEMBLEMULTIPLY_H

#include "../utility.h"
#include "tokeniser.h"

#define A_SHIFT 21
#define S_SHIFT 20
#define FILLER_SHIFT_MUL 4
#define RD_SHIFT_MUL 16
#define RS_SHIFT_MUL 8
#define RN_SHIFT_MUL 12

int32_t assembleMultiply(char* instruction);

#endif
