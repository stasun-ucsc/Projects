SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: encode decode

encode: encode.o trie.o word.o io.o
	$(CC) -o encode encode.o trie.o word.o io.o

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

decode: decode.o trie.o word.o io.o
	$(CC) -o decode decode.o trie.o word.o io.o

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

trie.o: trie.c
	$(CC) $(CFLAGS) -c trie.c

word.o: word.c
	$(CC) $(CFLAGS) -c word.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

clean:
	rm -f *.o encode decode

format:
	clang-format -i -style=file *.[ch]

