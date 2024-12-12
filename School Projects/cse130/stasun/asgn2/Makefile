SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic -gdwarf-4

all: httpserver

httpserver: httpserver.o
	$(CC) -o httpserver httpserver.o asgn2_helper_funcs.a

httpserver.o: httpserver.c
	$(CC) $(CFLAGS) -c httpserver.c

clean:
	rm -f *.o httpserver

format:
	clang-format -i -style=file *.[ch]


