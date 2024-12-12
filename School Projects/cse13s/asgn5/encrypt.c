#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "ss.h"
#include "randstate.h"

#define OPTIONS "i:o:n:vh"

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

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t flags = set_empty();
    FILE *infile = stdin, *outfile = stdout, *pbfile;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                printf("%s: No such file or directory\n", optarg);
                return 0;
            }
            break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n':
            pbfile = fopen(optarg, "r");
            if (pbfile == NULL) {
                printf("%s: No such file or directory\n", optarg);
                return 0;
            }
            flags = set_insert(flags, 1);
            break;
        case 'v': flags = set_insert(flags, 2); break;
        case 'h': flags = set_insert(flags, 0); break;
        default: flags = set_insert(flags, 0);
        }
    }
    if (set_member(flags, 0)) {
        printf("SYNOPSIS\n   Encrypts data using SS encryption.\n");
        printf("   Encrypted data is decrypted by the decrypt program.\n\n");
        printf("USAGE\n   ./encrypt [OPTIONS]\n\n");
        printf("OPTIONS\n   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
        printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
        printf("   -n pbfile       Public key file (default: ss.pub).\n");
        return 0;
    }

    if (!set_member(flags, 1)) {
        pbfile = fopen("ss.pub", "r");
        if (pbfile == NULL) {
            printf("ss.pub: No such file or directory\n");
            return 0;
        }
    }

    char username[100]; /* username not scanning correctly */
    mpz_t n;
    mpz_inits(n, NULL);

    ss_read_pub(n, username, pbfile);

    if (set_member(flags, 2)) {
        printf("user = %s\n", username);
        gmp_printf("n  (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    if (mpz_sizeinbase(n, 2) < 32) {
        fclose(infile);
        fclose(outfile);
        fclose(pbfile);
        mpz_clears(n, NULL);
        return 0;
    }

    ss_encrypt_file(infile, outfile, n);

    fclose(infile);
    fclose(outfile);
    fclose(pbfile);
    mpz_clears(n, NULL);

    return 0;
}
