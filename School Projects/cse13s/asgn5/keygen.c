#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "ss.h"
#include "randstate.h"

#define OPTIONS "b:i:n:d:s:vh"

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
    uint64_t nbits = 256, iters = 50, seed = time(NULL);
    FILE *pbfile, *pvfile;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': nbits = atoi(optarg); break;
        case 'i': iters = atoi(optarg); break;
        case 'n':
            pbfile = fopen(optarg, "w");
            flags = set_insert(flags, 1);
            break;
        case 'd':
            pvfile = fopen(optarg, "w");
            flags = set_insert(flags, 2);
            break;
        case 's': seed = atoi(optarg); break;
        case 'v': flags = set_insert(flags, 3); break;
        case 'h': flags = set_insert(flags, 0); break;
        default: flags = set_insert(flags, 0);
        }
    }
    if (set_member(flags, 0)) {
        printf("SYNOPSIS\n   Generates an SS public/private key pair.\n\n");
        printf("USAGE\n   ./keygen [OPTIONS]\n\n");
        printf("OPTIONS\n   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -b bits         Minimum bits needed for public key n (default: 256).\n");
        printf("   -i iterations   Miller-Rabin iterations for testing primes (default: 50).");
        printf("\n   -n pbfile       Public key file (default: ss.pub).\n");
        printf("   -d pvfile       Private key file (default: ss.priv).\n");
        printf("   -s seed         Random seed for testing.\n");
        return 0;
    }

    if (!set_member(flags, 1)) { /* need to error check here for the files */
        pbfile = fopen("ss.pub", "w");
    }

    if (!set_member(flags, 2)) {
        pvfile = fopen("ss.priv", "w");
    }

    fchmod(fileno(pvfile), 0600);

    randstate_init(seed);

    mpz_t p, q, n, d, pq;
    mpz_inits(p, q, n, d, pq, NULL);

    ss_make_pub(p, q, n, nbits, iters);
    ss_make_priv(d, pq, p, q);

    char *username = getenv("USER");

    ss_write_pub(n, username, pbfile);
    ss_write_priv(pq, d, pvfile);

    if (set_member(flags, 3)) {
        printf("user = %s\n", username);
        gmp_printf("p  (%lu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q  (%lu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n  (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("pq (%lu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%lu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    mpz_clears(p, q, n, d, pq, NULL);

    return 0;
}
