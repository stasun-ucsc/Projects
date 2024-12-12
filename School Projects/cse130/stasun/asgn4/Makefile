SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic -gdwarf-4

all: httpserver

httpserver: httpserver.o list.o
	$(CC) -o httpserver httpserver.o list.o asgn4_helper_funcs.a

httpserver.o: httpserver.c
	$(CC) $(CFLAGS) -c httpserver.c

list.o: list.c
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f *.o httpserver

format:
	clang-format -i -style=file *.[ch]


