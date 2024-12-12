SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: life

life: life.o universe.o
	$(CC) -o life life.o universe.o -lncurses

life.o: life.c
	$(CC) $(CFLAGS) -c life.c 

universe.o: universe.c
	$(CC) $(CFLAGS) -c universe.c

clean:
	rm -f *.o life universe

format:
	clang-format -i -style=file *.[ch]
