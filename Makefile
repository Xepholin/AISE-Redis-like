CC = gcc
OFLAGS = -Wall -O3

.PHONY: clean

all:
	$(CC) $(OFLAGS) src/main.c

clean:
	rm -f *.o
	rm -f a.out