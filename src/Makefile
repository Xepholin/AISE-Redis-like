CC = gcc
OFLAGS = -Wall

.PHONY: clean

all: server


server: parser.o command.o server.o
	$(CC) $(OFLAGS) $^ -o $@ -lm

%.o: %.c
	$(CC) $(OFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f server


