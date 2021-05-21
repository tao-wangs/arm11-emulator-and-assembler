#include <stdlib.h>
#include <stdio.h>

#define MEM_CAPACITY 65536
#define NUM_REGISTERS 17

#define SP 13
#define LR 14
#define PC 15
#define CPSR 16

//data structure to capture the state of the ARM machine
typedef struct {
    int memory[MEM_CAPACITY];
    int registers[NUM_REGISTERS];
} ARM_STATE;

void initialise(ARM_STATE state);
void terminate(ARM_STATE state);

//sets value of all registers to 0 
//sets value of all memory locations to 0
void initialise(ARM_STATE state) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        state.registers[i] = 0;
    }

    for (int i = 0; i < MEM_CAPACITY; i++) {
        state.memory[i] = 0;
    }
}

/*if an all-0 instruction is met then the emulator will terminate, 
this function will be called which prints out the contents of the registers and non-zero memory locations
*/
void terminate(ARM_STATE state) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("$%d: %x\n", i, state.registers[i]);
    }

    for (int i = 0; i < MEM_CAPACITY; i++) {
        if (state.memory[i] != 0) {
            printf("0x%x: %x", i * 4, state.memory[i]);
        }
    }
}