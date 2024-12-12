SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic

all: queue.o rwlock.o

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

rwlock.o: rwlock.c
	$(CC) $(CFLAGS) -c rwlock.c

clean:
	rm -f *.o *.bin *.dat

format:
	clang-format -i -style=file *.[ch]


