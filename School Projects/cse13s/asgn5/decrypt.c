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
    FILE *infile = stdin, *outfile = stdout, *pvfile;

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
            pvfile = fopen(optarg, "r");
            if (pvfile == NULL) {
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
        printf("SYNOPSIS\n   Decrypts data using SS decryption.\n");
        printf("   Encrypted data is encrypted by the encrypt program.\n\n");
        printf("USAGE\n   ./decrypt [OPTIONS]\n\n");
        printf("OPTIONS\n   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -i infile       Input file of data to decrypt (default: stdin).\n");
        printf("   -o outfile      Output file for decrypted data (default: stdout).\n");
        printf("   -n pvfile       Private key file (default: ss.priv).\n");
        return 0;
    }

    if (!set_member(flags, 1)) {
        pvfile = fopen("ss.priv", "r");
        if (pvfile == NULL) {
            printf("%s: No such file or directory\n", optarg);
            return 0;
        }
    }

    mpz_t pq, d;
    mpz_inits(pq, d, NULL);

    ss_read_priv(pq, d, pvfile);

    if (set_member(flags, 2)) {
        gmp_printf("pq (%lu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%lu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    ss_decrypt_file(infile, outfile, d, pq);

    fclose(infile);
    fclose(outfile);
    fclose(pvfile);
    mpz_clears(pq, d, NULL);

    return 0;
}
