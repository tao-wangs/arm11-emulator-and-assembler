#ifndef ARM_STATE_H
#define ARM_STATE_H

#define MEM_CAPACITY 65536
#define NUM_ALLIGNED_ADDRS 16384
#define NUM_REGISTERS 17
#define WORD_SIZE 4

#define SP 13
#define LR 14

//data structure to capture the state of the ARM machine
typedef struct {
    int32_t *memory;
    int32_t registers[NUM_REGISTERS];
} ARM_STATE;

void initialise(ARM_STATE *state);
void terminate(ARM_STATE *state);

#endif
