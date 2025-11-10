CFLAGS=-O2 -Wall -std=c23 -m32
LDFLAGS=-lbu

.PHONY: all clean

all: clean bin/chrisping

bin:
	mkdir -p bin

bin/chrisping: bin/chrisping.o | bin
	cc $(CFLAGS) $^ -o $@ $(LDFLAGS)

bin/chrisping.o: chrisping.c chrisping.h | bin
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f ./bin/*.o ./bin/chrisping