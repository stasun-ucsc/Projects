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

    struct stat in_stats;

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
        printf("SYNOPSIS\n   Compresses files using the LZ78 compression algorithm.\n");
        printf("   Compressed files are decompressed with the corresponding decoder.\n\n");
        printf("USAGE\n   ./encode [-vh] [-i input] [-o output]\n\n");
        printf("OPTIONS\n   -v          Display compression statistics\n   ");
        printf("-i input    Specify input to compress (stdin by default)\n   ");
        printf("-o output   Specify output of compressed input (stdout by default)\n   ");
        printf("-h          Display program help and usage\n");

        return 0;
    }

    fstat(infile, &in_stats);

    FileHeader *header = (FileHeader *) calloc(1, sizeof(FileHeader));
    header->magic = MAGIC;
    header->protection = in_stats.st_mode;
    int written = 0;

    fchmod(outfile, header->protection);

    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    uint8_t curr_sym = 0, prev_sym = 0;
    uint16_t next_code = START_CODE;

    while (read_sym(infile, &curr_sym)) {
        if (!written) {
            write_header(outfile, header);
            written = 1;
        }
        TrieNode *next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, bit_length(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code++;
        }
        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
    }
    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, bit_length(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, 0, bit_length(next_code));
    flush_pairs(outfile);

    if (set_member(flags, 1)) {
        double pct = 100.0 * (1.0 - ((double) (ceil_div(total_bits, 8) + 8) / (double) total_syms));
        fprintf(stderr, "Compressed file size: %u bytes\n", ceil_div(total_bits, 8) + 8);
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", total_syms);
        fprintf(stderr, "Compression ratio: %4.2f%%\n", pct);
    }

    trie_delete(root);
    free(header);

    close(infile);
    close(outfile);

    return 0;
}
