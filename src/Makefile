CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99
#CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic
#above is default flags 

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o assembler/hash.c assembler/firstpass.c assembler/assemblebranch.c assembler/secondpass.c assembler/tokeniser.c assembler/assdataprocessing.c assembler/assembleSDT.c assembler/assemblemultiply.c assembler/binfilewriter.c utility.c
	$(CC) $(CFLAGS) $^ -o $@

emulate: emulate.o emulator/decode.c emulator/binreader.c emulator/multiply.c emulator/pipeline.c emulator/arm_state.c emulator/branch.c emulator/singledatatransfer.c emulator/dataprocessing.c utility.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
