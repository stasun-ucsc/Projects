#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "trie.h"
#include "word.h"
#include "io.h"
#include "code.h"

#define OPTIONS "vi:o:h"

typedef uint32_t Set;

uint32_t set_empty(void);

uint32_t set_insert(Set s, uint8_t x);

bool set_member(Set s, uint8_t x);

uint32_t set_empty(void) {
    return 0x00000000;
}

uint32_t set_insert(Set s, uint8_t x) {
    return s | (1 << x);
}

bool set_member(Set s, uint8_t x) {
    if ((s & (1 << x)) == set_empty()) {
        return false;
    } else {
        return true;
    }
}

int bit_length(uint16_t n) {
    int len = 0;
    while (n > 0) {
        n = n / 2;
        len++;
    }
    return len;
}

unsigned int ceil_div(unsigned int n, unsigned int d) {
    if (n % d == 0) {
        return n / d;
    } else {
        return (n / d) + 1;
    }
}

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t flags = set_empty();
    int infile = 0, outfile = 1;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = open(optarg, O_RDONLY, 0);
            if (infile == -1) {
                printf("%s: No such file or directory\n", optarg);
                return 0;
            }
            break;
        case 'o':
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            if (infile == -1) {
                printf("%s: No such file or directory\n", optarg);
                return 0;
            }
            break;
        case 'v': flags = set_insert(flags, 1); break;
        case 'h': flags = set_insert(flags, 0); break;
        default: flags = set_insert(flags, 0);
        }
    }

    if (set_member(flags, 0)) {
        printf("SYNOPSIS\n   Decompresses files with the LZ78 decompression algorithm.\n");
        printf("   Used with files compressed with the corresponding encoder.\n\n");
        printf("USAGE\n   ./decode [-vh] [-i input] [-o output]\n\n");
        printf("OPTIONS\n   -v          Display decompression statistics\n   ");
        printf("-i input    Specify input to decompress (stdin by default)\n   ");
        printf("-o output   Specify output of decompressed input (stdout by default)\n   ");
        printf("-h          Display program usage\n");

        return 0;
    }

    FileHeader *header = (FileHeader *) calloc(1, sizeof(FileHeader));
    read_header(infile, header);
    if (header->magic != MAGIC) {
        fprintf(stderr, "Bad magic number.\n");
        return 0;
    }
    fchmod(outfile, header->protection);

    WordTable *table = wt_create();
    uint8_t curr_sym = 0;
    uint16_t curr_code = 0, next_code = START_CODE;
    while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code))) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        next_code++;
        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }

    flush_words(outfile);

    if (set_member(flags, 1)) {
        double pct = 100.0 * (1.0 - ((double) (ceil_div(total_bits, 8) + 8) / (double) total_syms));
        fprintf(stderr, "Compressed file size: %u bytes\n", ceil_div(total_bits, 8) + 8);
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", total_syms);
        fprintf(stderr, "Compression ratio: %4.2f%%\n", pct);
    }

    wt_delete(table);
    free(header);

    close(infile);
    close(outfile);

    return 0;
}
