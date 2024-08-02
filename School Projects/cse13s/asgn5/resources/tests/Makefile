TEST_NUMTHEORY = test_numtheory
TEST_NUMTHEORY_OBJ = test_numtheory.o
TEST_SS = test_ss
TEST_SS_OBJ = test_ss.o
TEST_BANNED = test_prohibited.sh
TESTS = $(TEST_NUMTHEORY) $(TEST_SS)
TEST_OBJS = $(TESTS:%=%.o)

SOURCES := $(wildcard ../*.c)
ASGN_SRC := ${shell grep -L "int main(" ${SOURCES}}
ASGN_OBJ = $(ASGN_SRC:%.c=%.o)

CC = clang

CHECK_CFLAGS = $(shell pkg-config --cflags check)
CHECK_LFLAGS = $(shell pkg-config --libs check)

GMP_CFLAGS = $(shell pkg-config --cflags gmp)
GMP_LFLAGS = $(shell pkg-config --libs gmp)

CFLAGS = $(CHECK_CFLAGS) $(GMP_CFLAGS) -Wno-format-invalid-specifier -Wno-format-extra-args -Wno-format
LFLAGS = $(CHECK_LFLAGS) $(GMP_LFLAGS)

all: run

run: $(TEST_NUMTHEORY) $(TEST_SS) $(TEST_BANNED)
	-./$(TEST_NUMTHEORY)
	-./$(TEST_SS)
	bash $(TEST_BANNED)

$(TEST_SS): $(ASGN_OBJ) $(TEST_SS_OBJ)
	$(CC) -o $@ $^ $(LFLAGS) -lm -lpthread

$(TEST_NUMTHEORY): $(ASGN_OBJ) $(TEST_NUMTHEORY_OBJ)
	$(CC) -o $@ $^ $(LFLAGS) -lm -lpthread

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(TESTS) $(TEST_OBJS)
	-rm -f *.tap
	-make clean -C ../
