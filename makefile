CC=gcc
CFLAGS=-Wall -w
SYNCOBJ=pe_sync.o
EXTLIBS=-lpthread -lrt

all: readers_writers_test one_slot_buffer_test child_care_test

part1: readers_writers_test one_slot_buffer_test
part2: child_care_test

clean:
	rm -f *.o *.bin

.PHONY: all clean

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

%_test: %_test.c %.o $(SYNCOBJ)
	$(CC) $(CFLAGS) $? -o $@.bin $(EXTLIBS)
