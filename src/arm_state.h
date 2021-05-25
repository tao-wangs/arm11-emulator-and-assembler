#ifndef ARM_STATE_H
#define ARM_STATE_H

#define MEM_CAPACITY 65536
#define NUM_ALLIGNED_ADDRS 16384
#define NUM_REGISTERS 17
#define WORD_SIZE 4

#define SP 13
#define LR 14
#define PC 15
#define CPSR 16

//data structure to capture the state of the ARM machine
typedef struct {
    int *memory;
    int registers[NUM_REGISTERS];
} ARM_STATE;

void initialise(ARM_STATE *state);
void terminate(ARM_STATE *state);

typedef enum {
    eq = "0000",
    ne = "0001",
    ge = "1010",
    lt = "1011",
    gt = "1100",
    le = "1101",
    al = "1110"
} COND_CODE;

typedef enum {
    and = "0000",
    eor = "0001",
    sub = "0010",
    rsb = "0011",
    add = "0100",
    tst = "1000",
    teq = "1001",
    cmp = "1010",
    orr = "1100",
    mov = "1101"
} OPCODE;

typedef enum {
    N = "1000",
    Z = "0100",
    C = "0010",
    V = "0001"
} FLAG;

#endif