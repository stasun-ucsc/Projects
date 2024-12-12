SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic

all: memory

memory: memory.o
	$(CC) -o memory memory.o

memory.o: memory.c
	$(CC) $(CFLAGS) -c memory.c

clean:
	rm -f *.o memory

format:
	clang-format -i -style=file *.[ch]


