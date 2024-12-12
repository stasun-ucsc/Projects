SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = `pkg-config --libs gmp`

all: encrypt decrypt keygen

encrypt: encrypt.o randstate.o numtheory.o ss.o
	$(CC) -o encrypt encrypt.o randstate.o numtheory.o ss.o $(LFLAGS)

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c

decrypt: decrypt.o randstate.o numtheory.o ss.o
	$(CC) -o decrypt decrypt.o randstate.o numtheory.o ss.o $(LFLAGS)

decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c 

keygen: keygen.o randstate.o numtheory.o ss.o
	$(CC) -o keygen keygen.o randstate.o numtheory.o ss.o $(LFLAGS)

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c

randstate.o: randstate.c
	$(CC) $(CFLAGS) -c randstate.c

numtheory.o: numtheory.c
	$(CC) $(CFLAGS) -c numtheory.c

ss.o: ss.c
	$(CC) $(CFLAGS) -c ss.c 

clean:
	rm -f *.o encrypt decrypt keygen

format:
	clang-format -i -style=file *.[ch]
