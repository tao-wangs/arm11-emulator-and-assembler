CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99
#CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic
#above is default flags 

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o decode.c binreader.c multiply.c pipeline.c arm_state.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
