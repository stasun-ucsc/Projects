SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: sorting 

sorting: sorting.o stats.o set.o batcher.o shell.o heap.o quick.o
	$(CC) -o sorting sorting.o stats.o set.o batcher.o shell.o heap.o quick.o

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c 

stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

batcher.o: batcher.c
	$(CC) $(CFLAGS) -c batcher.c

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c

clean:
	rm -f *.o sorting stats set batcher shell heap quick

format:
	clang-format -i -style=file *.[ch]

