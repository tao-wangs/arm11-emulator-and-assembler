#include <stdlib.h>
#include <stdio.h>
#include "arm_state.h"

void initialise(ARM_STATE *state);
void terminate(ARM_STATE *state);

//sets value of all registers to 0 
//sets value of all memory locations to 0
void initialise(ARM_STATE *state) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        state->registers[i] = 0;
    }

    for (int i = 0; i < MEM_CAPACITY; i++) {
        state->memory[i] = 0;
    }
}

/*if an all-0 instruction is met then the emulator will terminate, 
this function will be called which prints out the contents of the registers and non-zero memory locations
*/
void terminate(ARM_STATE *state) {
    printf("Registers:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("$%d: %x\n", i, state->registers[i]);
    }
    printf("Non-zero memory:\n");
    for (int i = 0; i < MEM_CAPACITY; i++) {
        if (state->memory[i] != 0) {
            printf("0x%x: %x\n", i * WORD_SIZE, state->memory[i]);
        }
    }
    printf("Emulator terminated.\n");
}