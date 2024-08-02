#include "../io.h"
#include <check.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define TESTS       "io_tests"
#define RESULT_FILE "results_io.tap"
#define BYTES       10000

START_TEST(test_header_read_write) {
    FileHeader in, out;
    int outfile = open("testfile.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    ck_assert_msg(outfile >= 0, "Error opening outfile");
    struct stat stats;
    fstat(outfile, &stats);
    fchmod(outfile, stats.st_mode);
    out.magic = MAGIC;
    out.protection = stats.st_mode;
    write_header(outfile, &out);
    int success = close(outfile);
    ck_assert_msg(success >= 0, "Error closing outfile");
    int infile = open("testfile.txt", O_RDONLY);
    ck_assert_msg(infile >= 0, "Error opening infile");
    int bytes = read_bytes(infile, (uint8_t *) &in, sizeof(FileHeader));
    ck_assert_msg(bytes == sizeof(FileHeader),
        "read_bytes did not retun enough bytes for header, read %d", bytes);
    ck_assert_msg(in.magic == MAGIC, "Incorrect magic number written, \nexpected: %x \ngot: %x",
        MAGIC, in.magic);
    ck_assert_msg(in.protection == stats.st_mode,
        "Incorrect protection bits written, \nexpected: %x \ngot: %x", stats.st_mode,
        in.protection);
    success = close(infile);
    ck_assert_msg(success >= 0, "Error closing outfile");
}
END_TEST

START_TEST(test_read_bytes) {
    FILE *outfile;
    uint8_t buffer[BYTES] = { 0 };
    for (int i = 0; i < BYTES; i += 1) {
        buffer[i] = (uint8_t) random();
    }
    outfile = fopen("readtest.txt", "w");
    fwrite(buffer, 1, BYTES, outfile);
    fclose(outfile);
    int infile = open("readtest.txt", O_RDONLY);
    ck_assert_msg(infile >= 0, "Unable to open readtest.txt");
    uint8_t readbuffer[BYTES];
    int bytes = read_bytes(infile, readbuffer, BYTES);
    ck_assert_msg(
        bytes == BYTES, "Incorrect number of bytes read, \nexpected: %d, \ngot: %d", BYTES, bytes);
    for (int i = 0; i < BYTES; i += 1) {
        ck_assert_msg(readbuffer[i] == buffer[i], "Read test failed, \nexpected: %x \ngot: %x",
            buffer[i], readbuffer[i]);
    }
    int result = close(infile);
    ck_assert_msg(result == 0, "Unable to close readtest.txt");
}
END_TEST

START_TEST(test_write_bytes) {
    FILE *infile;
    uint8_t buffer[BYTES] = { 0 };
    for (int i = 0; i < BYTES; i += 1) {
        buffer[i] = (uint8_t) random();
    }
    int outfile = open("writetest.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ck_assert_msg(outfile >= 0, "Unable to open writetest.txt");
    int bytes = write_bytes(outfile, buffer, BYTES);
    ck_assert_msg(bytes == BYTES, "Incorrect number of bytes written, \nexpected: %d, \ngot: %d",
        BYTES, bytes);
    int result = close(outfile);
    ck_assert_msg(result == 0, "Unable to close writetest.txt");
    uint8_t readbuffer[BYTES];
    infile = fopen("writetest.txt", "r");
    fread(readbuffer, 1, BYTES, infile);
    for (int i = 0; i < BYTES; i += 1) {
        ck_assert_msg(readbuffer[i] == buffer[i], "Read test failed, \nexpected: %x \ngot: %x",
            buffer[i], readbuffer[i]);
    }
    fclose(infile);
}
END_TEST

START_TEST(test_read_pair) {
    FILE *outfile = fopen("pairs.txt", "w");
    uint8_t buffer[256] = { 0 };
    int nextcode = 1;
    for (int i = 0; i < 128; i++) {
        nextcode += 1;
        buffer[2 * i] = (uint8_t) nextcode;
        buffer[2 * i + 1] = (uint8_t) random();
        if (i == 127) {
            buffer[2 * i] = 0;
        }
    }
    fwrite(buffer, 1, 256, outfile);
    fclose(outfile);

    int infile = open("pairs.txt", O_RDONLY);
    ck_assert_msg(infile >= 0, "Error reading created file");
    uint16_t code;
    uint8_t sym;
    uint16_t count = 0;
    while (read_pair(infile, &code, &sym, 8)) {
        ck_assert_msg(code == (uint8_t) buffer[2 * count],
            "Read incorrect code, expected %d got %d", buffer[2 * count], code);
        ck_assert_msg(sym == buffer[2 * count + 1], "Read incorrect symbol, expected %c, got %c",
            buffer[2 * count + 1], sym);
        count += 1;
    }
    ck_assert_msg(count == 127, "Incorrect number of pairs read, expected 127, got %d", count);
    int result = close(infile);
    ck_assert_msg(result == 0, "Unable to close file");
}
END_TEST

START_TEST(test_write_pair) {
    int outfile = open("writepair.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int nextcode = 1;
    uint16_t code[128];
    uint8_t sym[128];
    for (int i = 0; i < 128; i++) {
        nextcode += 1;
        code[i] = nextcode;
        sym[i] = (uint8_t) random();
        write_pair(outfile, code[i], sym[i], 8);
    }
    flush_pairs(outfile);
    int result = close(outfile);
    ck_assert_msg(result == 0, "Unable to close file");

    FILE *infile = fopen("writepair.txt", "r");
    uint8_t buffer[256] = { 0 };
    size_t readbytes = fread(buffer, 1, 256, infile);
    ck_assert_msg(readbytes == 256, "Expected 128 pairs, got %zu", readbytes / 2);
    for (int i = 0; i < 128; i++) {
        ck_assert_msg(buffer[2 * i] == code[i],
            "Read and written codes do not match, expected %d got %d", code[i], buffer[2 * i]);
        ck_assert_msg(buffer[2 * i + 1] == sym[i],
            "Read and written syms do not match, expected %d got %d", sym[i], buffer[2 * i + 1]);
    }
    fclose(infile);
}
END_TEST

START_TEST(test_write_word) {
    WordTable *table = wt_create();
    uint8_t syms[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
    int next = 2;
    int outfile = open("words.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ck_assert_msg(outfile >= 0, "Unable to open words.txt");
    for (int i = 1; i < 11; i++) {
        table[next] = word_append_sym(table[i], syms[i - 1]);
        ck_assert_msg(table[next], "table has no element %d", next);
        write_word(outfile, table[next]);
        next = next + 1;
    }
    flush_words(outfile);
    wt_delete(table);
    int result = close(outfile);
    ck_assert_msg(result == 0, "Error closing file");
    FILE *infile = fopen("words.txt", "r");
    uint8_t expected[] = "aababcabcdabcdeabcdefabcdefgabcdefghabcdefghiabcdefghij";
    uint8_t buffer[60] = { 0 };
    fread(buffer, 1, 60, infile);
    int same = memcmp(expected, buffer, sizeof(expected));
    ck_assert_msg(
        same == 0, "Strings different with write_word \nexpected: %s\ngot: %s", expected, buffer);
    fclose(infile);
}

Suite *test_suite(void) {
    Suite *suite = suite_create("");

    TCase *tests = tcase_create(TESTS);
    tcase_add_test(tests, test_header_read_write);
    tcase_add_test(tests, test_read_bytes);
    tcase_add_test(tests, test_write_bytes);
    tcase_add_test(tests, test_read_pair);
    tcase_add_test(tests, test_write_pair);
    tcase_add_test(tests, test_write_word);
    suite_add_tcase(suite, tests);
    return suite;
}

int main(void) {
    Suite *s = test_suite();
    SRunner *runner = srunner_create(s);
    srunner_set_fork_status(runner, CK_FORK);
    srunner_set_tap(runner, RESULT_FILE);

    srunner_run_all(runner, CK_VERBOSE);
    int no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
