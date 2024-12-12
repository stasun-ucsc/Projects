#include "randstate.h"
#include "numtheory.h"

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t temp, a_temp, b_temp;
    mpz_inits(temp, a_temp, b_temp, NULL);
    /* mpz_init(temp);
	mpz_init(a_temp);
	mpz_init(b_temp); */
    mpz_set(a_temp, a);
    mpz_set(b_temp, b);
    while (mpz_cmp_ui(b_temp, 0) != 0) {
        mpz_set(temp, b_temp);
        mpz_mod(b_temp, a_temp, b_temp);
        mpz_set(a_temp, temp);
    }
    mpz_set(g, a_temp);
    mpz_clears(temp, a_temp, b_temp, NULL);
    /* mpz_clear(temp);
	mpz_clear(a_temp);
	mpz_clear(b_temp); */
    return;
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, r_prime, t, t_prime, q, temp_tr, temp;
    mpz_inits(r, r_prime, t, t_prime, q, temp_tr, temp, NULL);
    mpz_set(r, n);
    mpz_set(r_prime, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(t_prime, 1);
    while (mpz_cmp_ui(r_prime, 0) != 0) {
        mpz_fdiv_q(q, r, r_prime);
        mpz_set(temp_tr, r);
        mpz_set(r, r_prime);
        mpz_mul(temp, q, r_prime);
        mpz_sub(r_prime, temp_tr, temp);
        mpz_set(temp_tr, t);
        mpz_set(t, t_prime);
        mpz_mul(temp, q, t_prime);
        mpz_sub(t_prime, temp_tr, temp);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
        mpz_clears(r, r_prime, t, t_prime, q, temp_tr, temp, NULL);
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(o, t);
    mpz_clears(r, r_prime, t, t_prime, q, temp_tr, temp, NULL);
    return;
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t p, temp, temp_d; /* look to optimize? no new vars? */
    mpz_init(temp);
    mpz_init(temp_d);
    mpz_inits(p, NULL);
    mpz_set(p, a);
    mpz_set_ui(o, 1);
    mpz_set(temp_d, d);
    while (mpz_cmp_ui(temp_d, 0) > 0) {
        if (mpz_odd_p(temp_d)) {
            mpz_mul(temp, o, p);
            mpz_mod(o, temp, n);
        }
        mpz_mul(temp, p, p); /* way to do this with out temp? and is mpz_powm_ui better? */
        mpz_mod(p, temp, n);
        mpz_fdiv_q_ui(temp_d, temp_d, 2);
    }
    mpz_clear(temp);
    mpz_clear(temp_d);
    mpz_clears(p, NULL);
    return;
}

bool is_prime(const mpz_t n, uint64_t iters) {
    int64_t s = 0;
    mpz_t r, y, a, n_minus_one, two;
    mpz_inits(r, y, a, n_minus_one, two, NULL);
    mpz_set_ui(two, 2);

    mpz_sub_ui(n_minus_one, n, 1);
    mpz_set(r, n_minus_one);

    /* gmp_printf("entrance: %Zd\n", n); */

    if (mpz_cmp_ui(n, 1) <= 0) {
        mpz_clears(r, y, a, n_minus_one, two, NULL);
        return false;
    } else if (mpz_cmp_ui(n, 2) == 0) {
        mpz_clears(r, y, a, n_minus_one, two, NULL);
        return true;
    }

    else if (mpz_even_p(n)) {
        mpz_clears(r, y, a, n_minus_one, two, NULL);
        return false;
    }

    while (mpz_even_p(r)) {
        mpz_fdiv_q_ui(r, r, 2);
        s++;
    }

    for (uint64_t i = 0; i < iters; i++) {
        /* a = random(2, 3, ..., n - 2) should this go to n - 1? */
        /* I changed to n - 1, ask */
        mpz_urandomm(a, state, n);
        while ((mpz_cmp_ui(a, 0) == 0) || (mpz_cmp_ui(a, 1) == 0)) {
            mpz_urandomm(a, state, n);
        }

        /* gmp_printf("a = %Zd\n", a); */

        pow_mod(y, a, r, n);

        /* gmp_printf("y = %Zd\n", y); */

        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n_minus_one) != 0)) {
            int64_t counter = 1;
            while ((counter <= (s - 1)) && (mpz_cmp(y, n_minus_one) != 0)) {
                /* gmp_printf("y = %Zd\n", y);
				gmp_printf("two = %Zd\n", two);
				gmp_printf("n = %Zd\n", n); */
                pow_mod(y, y, two, n);
                /* gmp_printf("y = %Zd\n", y); */
                if (mpz_cmp_ui(y, 1) == 0) { /* != originally ==, double check */
                    mpz_clears(r, y, a, n_minus_one, two, NULL);
                    return false;
                }
                counter++;
            }
            if (mpz_cmp(y, n_minus_one) != 0) {
                mpz_clears(r, y, a, n_minus_one, two, NULL);
                return false; /* can this check be in loop? */
            }
        }
    }
    mpz_clears(r, y, a, n_minus_one, two, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    /* mpz_set_ui(p, bits);
	mpz_set_ui(p, iters); */
    /* printf("bits = %lu\n", bits); */
    mpz_urandomb(p, state, bits);
    while (!(is_prime(p, iters))) {
        mpz_urandomb(p, state, bits);
    }
    /* gmp_printf("p = %Zd\n", p); */
    return;
}
