SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -gdwarf-4

all: mathlib-test

mathlib-test: mathlib-test.o e.o bbp.o madhava.o euler.o viete.o newton.o
	$(CC) -o mathlib-test mathlib-test.o e.o bbp.o madhava.o euler.o viete.o newton.o

mathlib-test.o: mathlib-test.c
	$(CC) $(CFLAGS) -c mathlib-test.c

e.o: e.c
	$(CC) $(CFLAGS) -c e.c

bbp.o: bbp.c
	$(CC) $(CFLAGS) -c bbp.c

madhava.o: madhava.c
	$(CC) $(CFLAGS) -c madhava.c

euler.o: euler.c
	$(CC) $(CFLAGS) -c euler.c

viete.o: viete.c
	$(CC) $(CFLAGS) -c viete.c

newton.o: newton.c
	$(CC) $(CFLAGS) -c newton.c

clean:
	rm -f *.o mathlib-test

format:
	clang-format -i -style=file *.[ch]


