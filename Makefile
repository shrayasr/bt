CC=gcc
CFLAGS=-Wall -std=c99 -g
DEPS=
OBJS=bencode.o bencode-test.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

bencode-test: $(OBJS) bencode.h
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.PHONY: clean

clean:
	rm -f *.o *.c~
	-rm -f bencode-test
test: bencode-test
	./bencode-test

leakcheck: bencode-test
	valgrind --leak-check=yes ./bencode-test
