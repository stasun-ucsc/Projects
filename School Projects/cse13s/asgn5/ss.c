#include <stdlib.h>
#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    mpz_t p_minus_one, q_minus_one, temp_r1, temp_r2;
    mpz_inits(p_minus_one, q_minus_one, temp_r1, temp_r2, NULL);
    uint64_t p_bits, q_bits, temp_n;
    temp_n = nbits;
    nbits += 4;
    p_bits = random() % (((2 * nbits) / 5) + 1);
    while (((nbits / 5) > p_bits) || (((2 * nbits) / 5) < p_bits) || (p_bits <= 1)) {
        p_bits = random() % (((2 * nbits) / 5) + 1);
    }
    q_bits = nbits - (2 * p_bits);
    /* printf("p_bits = %lu, q_bits = %lu\n", p_bits, q_bits); */
    make_prime(p, p_bits, iters);
    make_prime(q, q_bits, iters);
    mpz_sub_ui(p_minus_one, p, 1);
    mpz_sub_ui(q_minus_one, q, 1);
    mpz_mod(temp_r1, q_minus_one, p);
    mpz_mod(temp_r2, p_minus_one, q);
    while ((mpz_cmp_ui(temp_r1, 0) == 0) || (mpz_cmp_ui(temp_r2, 0)) == 0) {
        make_prime(p, p_bits, iters);
        mpz_sub_ui(p_minus_one, p, 1);
        make_prime(q, q_bits, iters);
        mpz_sub_ui(q_minus_one, q, 1);
        /* gmp_printf("p = %Zd, q = %Zd\n", p, q); */
        mpz_mod(temp_r1, q_minus_one, p);
        mpz_mod(temp_r2, p_minus_one, q);
        /* gmp_printf("p-1 mod q = %Zd, q-1 mod p = %Zd\n", temp_r2, temp_r1); */
    }
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
    mpz_clears(p_minus_one, q_minus_one, temp_r1, temp_r2, NULL);
    if (mpz_sizeinbase(n, 2) < temp_n) {
        ss_make_pub(p, q, n, nbits, iters);
    }
    return;
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t p_minus_one, q_minus_one, n, temp;
    mpz_inits(p_minus_one, q_minus_one, n, temp, NULL);
    mpz_sub_ui(p_minus_one, p, 1);
    mpz_sub_ui(q_minus_one, q, 1);
    /* gmp_printf("p-1 = %Zd, q-1 = %Zd\n", p_minus_one, q_minus_one); */
    gcd(temp, p_minus_one, q_minus_one);
    /* gmp_printf("gcd = %Zd\n", temp); */
    mpz_mul(pq, p_minus_one, q_minus_one);
    mpz_fdiv_q(pq, pq, temp);
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
    mod_inverse(d, n, pq);
    mpz_mul(pq, p, q);
    mpz_clears(p_minus_one, q_minus_one, n, temp, NULL);
    return;
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
    /* fprintf(pbfile, "%s\n", username); */
    return;
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
    return;
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    fscanf(pbfile, "%99s\n", username);
    return;
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", pq, d);
    return;
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
    return;
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t m;
    mpz_inits(m, NULL);
    uint64_t j = 0;
    /* uint64_t k = mpz_sizeinbase(n, 2); */
    uint64_t k = (((mpz_sizeinbase(n, 2) / 2) - 1) / 8);
    if (k < 2) {
        k = 2;
    }
    uint8_t *block = (uint8_t *) calloc(k, 1);
    uint8_t new_byte;
    block[0] = 0xFF;
    while (fscanf(infile, "%c", &new_byte) == 1) {
        j++;
        if (j < k) {
            block[j] = new_byte;
        } else {
            mpz_import(m, j, 1, 1, 1, 0, block);
            ss_encrypt(m, m, n);
            gmp_fprintf(outfile, "%Zx\n", m);
            j = 1;
            block[0] = 0xFF;
            block[j] = new_byte;
        }
    }
    mpz_import(m, j, 1, 1, 1, 0, block);
    ss_encrypt(m, m, n);
    gmp_fprintf(outfile, "%Zx\n", m);
    mpz_clears(m, NULL);
    free(block);
    return;
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
    return;
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t c;
    mpz_inits(c, NULL);
    uint64_t j;
    uint64_t k = (mpz_sizeinbase(pq, 2) - 1) / 8;
    if (k < 2) {
        k = 2;
    }
    uint8_t *block = (uint8_t *) calloc(k, 1);
    while (gmp_fscanf(infile, "%Zx\n", c) == 1) {
        /* gmp_printf("read encrypted = %Zx\n", c); */
        ss_decrypt(c, c, d, pq);
        /* gmp_printf("decrypted = %Zd\n", c); */
        mpz_export(block, &j, 1, 1, 1, 0, c); /* might complain about j*/
        /* printf("j = %lu\n", j); */
        for (uint64_t counter = 1; counter < j; counter++) {
            fprintf(outfile, "%c", block[counter]);
        }
    }
    /* fprintf(outfile, "\n"); */
    mpz_clears(c, NULL);
    free(block);
    return;
}
