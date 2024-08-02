#include <stdio.h>
#include <gmp.h>
#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include "../numtheory.h"
#include "../randstate.h"

#define SEED        0xdead00d
#define TESTS       "numtheory_tests"
#define RESULT_FILE "results_numtheory.tap"

void setup(void) {
    randstate_init(SEED);
}

void teardown(void) {
    randstate_clear();
}

START_TEST(test_gcd) {
    mpz_t m, n, a, b;
    mpz_inits(m, n, a, b, NULL);

    // A couple random cases with 512-bit numbers.
    for (size_t i = 0; i < 5; i += 1) {
        mpz_rrandomb(a, state, 512);
        mpz_rrandomb(b, state, 512);

        mpz_gcd(m, a, b); // m holds expected GCD.
        gcd(n, a, b); // n holds student computed GCD.

        char *a_str = mpz_get_str(NULL, 10, a);
        char *b_str = mpz_get_str(NULL, 10, b);
        char *m_str = mpz_get_str(NULL, 10, m);
        char *n_str = mpz_get_str(NULL, 10, n);

        ck_assert_msg(
            mpz_cmp(m, n) == 0, "gcd(%s, %s): expected=%s, got=%s\n", a_str, b_str, m_str, n_str);

        free(a_str);
        free(b_str);
        free(m_str);
        free(n_str);
    }

    // Some fixed test cases.
    // Each test is a, b, then expected result.
    char *tests[5][3] = {
        { "30", "18", "6" },
        { "7", "7", "7" },
        { "20", "100", "20" },
        { "624129", "2061517", "18913" },
        { "37", "600", "1" },
    };

    for (size_t i = 0; i < 5; i += 1) {
        mpz_set_str(a, tests[i][0], 10);
        mpz_set_str(b, tests[i][1], 10);
        mpz_set_str(m, tests[i][2], 10);
        gcd(n, a, b);

        char *a_str = mpz_get_str(NULL, 10, a);
        char *b_str = mpz_get_str(NULL, 10, b);
        char *m_str = mpz_get_str(NULL, 10, m);
        char *n_str = mpz_get_str(NULL, 10, n);

        ck_assert_msg(
            mpz_cmp(m, n) == 0, "gcd(%s, %s): expected=%s, got=%s\n", a_str, b_str, m_str, n_str);

        free(a_str);
        free(b_str);
        free(m_str);
        free(n_str);
    }

    mpz_clears(m, n, a, b, NULL);
}
END_TEST

START_TEST(test_mod_inverse) {
    mpz_t p, q, a, n;
    mpz_inits(p, q, a, n, NULL);

    // Some fixed test cases.
    // Each test is a, b, then expected result.
    char *tests[10][3] = {
        { "5", "13", "8" }, { "1", "2", "1" }, { "3", "6", "0" }, // No inverse.
        { "7", "87", "25" }, { "25", "87", "7" }, { "2", "91", "46" },
        { "13", "91", "0" }, // No inverse.
        { "19", "1212393831", "701912218" }, { "31", "73714876143", "45180085378" },
        { "3", "73714876143", "0" }, // No inverse.
    };

    for (size_t i = 0; i < 10; i += 1) {
        mpz_set_str(a, tests[i][0], 10);
        mpz_set_str(n, tests[i][1], 10);
        mpz_set_str(p, tests[i][2], 10);
        mod_inverse(q, a, n);

        char *a_str = mpz_get_str(NULL, 10, a);
        char *n_str = mpz_get_str(NULL, 10, n);
        char *p_str = mpz_get_str(NULL, 10, p);
        char *q_str = mpz_get_str(NULL, 10, q);

        ck_assert_msg(mpz_cmp(p, q) == 0, "mod_inverse(%s, %s): expected=%s, got=%s\n", a_str,
            n_str, p_str, q_str);

        free(a_str);
        free(n_str);
        free(p_str);
        free(q_str);
    }

    mpz_clears(p, q, a, n, NULL);
}
END_TEST

START_TEST(test_pow_mod) {
    mpz_t p, q, a, d, n;
    mpz_inits(p, q, a, d, n, NULL);

    // Some fixed test cases.
    // Each test is a, d, n, then expected result.
    char *tests[10][4] = {
        { "2", "8", "255", "1" },
        { "2", "8", "256", "0" },
        { "2", "8", "257", "256" },
        { "3", "7", "10000", "2187" },
        { "2", "2046", "2047", "1" },
        { "123", "456", "789", "699" },
        { "3", "1000", "18446744073709551615", "12311760789144243126" },
        { "86400", "22157322", "48519018006822", "40149207423504" },
        {
            "8675309",
            "100018327824",
            "8621993634251008000",
            "3858055581225668161",
        },
        {
            "325284989554104320",
            "1508436685178379520",
            "8582294829391072256",
            "6354230931838838784",
        },
    };

    for (size_t i = 0; i < 10; i += 1) {
        mpz_set_str(a, tests[i][0], 10);
        mpz_set_str(d, tests[i][1], 10);
        mpz_set_str(n, tests[i][2], 10);
        mpz_set_str(p, tests[i][3], 10);
        pow_mod(q, a, d, n);

        char *a_str = mpz_get_str(NULL, 10, a);
        char *d_str = mpz_get_str(NULL, 10, d);
        char *n_str = mpz_get_str(NULL, 10, n);
        char *p_str = mpz_get_str(NULL, 10, p);
        char *q_str = mpz_get_str(NULL, 10, q);

        ck_assert_msg(mpz_cmp(p, q) == 0, "pow_mod(%s, %s, %s): expected=%s, got=%s\n", a_str,
            d_str, n_str, p_str, q_str);

        free(a_str);
        free(d_str);
        free(n_str);
        free(p_str);
        free(q_str);
    }

    mpz_clears(p, q, a, d, n, NULL);
}
END_TEST

START_TEST(test_is_prime) {
    mpz_t n;
    mpz_init(n);

    char *tests[18] = {
        "0",
        "1",
        "2",
        "3",
        "4",
        "41041",
        "46657",
        "52633",
        "62745",
        "63973",
        "252601",
        "3057601",
        "104717",
        "577757",
        "101089",
        "280001",
        "100000004677",
        "100000004678",
    };

    bool results[18] = { false, false, true, true, false, false, false, false, false, false, false,
        false, true, true, true, true, true, false };

    // 50 iterations of Miller-Rabin should suffice.
    // The second print must be the opposite of the expected result.
    // This is done since is_prime() is a probabilistic test.
    for (size_t i = 0; i < 18; i += 1) {
        mpz_set_str(n, tests[i], 10);

        char *n_str = mpz_get_str(NULL, 10, n);

        ck_assert_msg(is_prime(n, 50) == results[i], "is_prime(%s): expected=%s, got=%s\n", n_str,
            results[i] ? "true" : "false", results[i] ? "false" : "true");

        free(n_str);
    }

    mpz_clear(n);
}
END_TEST

START_TEST(test_make_prime) {
    mpz_t p;
    mpz_init(p);

    uint64_t bits[4] = { 128, 256, 512, 1024 };

    // 50 iterations of Miller-Rabin should suffice.
    for (size_t i = 0; i < 4; i += 1) {
        make_prime(p, bits[i], 50);

        char *p_str = mpz_get_str(NULL, 10, p);

        ck_assert_msg(mpz_probab_prime_p(p, 50) > 0, "%s is not prime", p_str);

        free(p_str);
    }

    mpz_clear(p);
}
END_TEST

Suite *test_suite(void) {
    Suite *suite = suite_create("");

    TCase *tests = tcase_create(TESTS);
    tcase_add_checked_fixture(tests, setup, teardown);
    tcase_add_test(tests, test_gcd);
    tcase_add_test(tests, test_mod_inverse);
    tcase_add_test(tests, test_pow_mod);
    tcase_add_test(tests, test_is_prime);
    tcase_add_test(tests, test_make_prime);

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
