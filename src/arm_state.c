#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "arm_state.h"

//allocates space for memory
//sets value of all registers to 0
//sets value of all memory locations to 0
void initialise(ARM_STATE *state) {
    state->memory = malloc(MEM_CAPACITY);

    for (int32_t i = 0; i < NUM_REGISTERS; i++) {
        state->registers[i] = 0;
    }

    for (int32_t i = 0; i < NUM_ALLIGNED_ADDRS; i++) {
        state->memory[i] = 0;
    }
}

/*if an all-0 instruction is met then the emulator will terminate,
this function will be called which prints out the contents of the registers and non-zero memory locations
*/
void terminate(ARM_STATE *state) {
    printf("Registers:\n");
    for (int32_t i = 0; i < NUM_REGISTERS; i++) {
        switch (i)
        {
            case 10:
            case 11:
            case 12:
                printf("$%d : %10d (0x%08x)\n", i, state->registers[i], state->registers[i]);
                break;
            case SP:
            case LR:
                break;
            case 15:
                printf("PC  : %10d (0x%08x)\n", state->registers[PC], state->registers[PC]);
                break;
            case 16:
                printf("CPSR: %10d (0x%08x)\n", state->registers[CPSR], state->registers[CPSR]);
                break;
            default:
                printf("$%d  : %10d (0x%08x)\n", i, state->registers[i], state->registers[i]);
        }
    }

    printf("Non-zero memory:\n");
    for (int32_t i = 0; i < NUM_ALLIGNED_ADDRS; i++) {
        if (state->memory[i] != 0) {
            printf("0x%08x: 0x%08x\n", i * WORD_SIZE, state->memory[i]);
        }
    }
}

