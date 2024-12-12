SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic -gdwarf-4 

all: cacher

cacher: cacher.o list.o
	$(CC) -o cacher cacher.o list.o

cacher.o: cacher.c
	$(CC) $(CFLAGS) -c cacher.c

list.o: list.c
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f *.o cacher

format:
	clang-format -i -style=file *.[ch]


