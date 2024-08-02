#include <stdio.h>
#include <gmp.h>
#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include "../numtheory.h"
#include "../randstate.h"
#include "../ss.h"

#define SEED        0xdeadd00d
#define TESTS       "ss_tests"
#define RESULT_FILE "results_ss.tap"

void setup(void) {
    randstate_init(SEED);
}

void teardown(void) {
    randstate_clear();
}

START_TEST(test_ss_make_keys) {
    uint64_t bits[5] = { 256, 120, 64, 126, 80 };
    uint64_t iters = 50;

    mpz_t p, q, pq, e, n, d, priv, pmo, qmo, prod, tot;
    mpz_inits(p, q, pq, n, d, priv, pmo, qmo, tot, prod, NULL);

    for (size_t i = 0; i < 5; i += 1) {
        ss_make_pub(p, q, n, bits[i], iters);
        ss_make_priv(d, pq, p, q);
        mpz_mul(prod, p, q);
        mpz_mul(prod, p, prod);
        mpz_mul(pq, p, q);

        ck_assert_msg(mpz_probab_prime_p(p, 50) > 0, "p is not prime\n");
        ck_assert_msg(mpz_probab_prime_p(q, 50) > 0, "q is not prime\n");
        ck_assert_msg(mpz_cmp(n, prod) == 0, "n is not the product of p^2 and q\n");
        uint64_t nbits = mpz_sizeinbase(n, 2);
        uint64_t pbits = mpz_sizeinbase(p, 2);
        uint64_t qbits = mpz_sizeinbase(q, 2);
        ck_assert_msg((2*pbits + qbits) >= nbits,
                "n is not the right number of bits, expected >= %lu got %lu \n",
                nbits, 2*pbits+qbits);

        char *student_key = mpz_get_str(NULL, 10, d);
        char *ref_key = mpz_get_str(NULL, 10, priv);

        mpz_sub_ui(pmo, p, 1);
        mpz_sub_ui(qmo, q, 1);
        mpz_lcm(tot, pmo, qmo);
        mpz_invert(priv, prod, tot);

        ck_assert_msg(mpz_cmp(d, priv) == 0,
            "private key not generated correctly, expected: %s, got: %s\n", ref_key, student_key);

        free(ref_key);
        free(student_key);
        mpz_inits(p, pmo, q, qmo, n, e, d, tot, prod, priv, NULL);
    }

    mpz_clears(p, pmo, q, qmo, n, e, d, tot, prod, priv, NULL);
}
END_TEST

START_TEST(test_ss_write_pub) {
    // Open public key for reading and writing (w+).
    FILE *file = fopen("/tmp/test_write_pub.pub", "w+");

    char *n_str = "227339540862705503616796633164928975737142997203907226968556531248115531244661";
    char *username = strdup("cse13s");

    mpz_t n, np, e, ep, s, sp;
    mpz_inits(n, np, e, ep, s, sp, NULL);
    mpz_set_str(n, n_str, 10);

    // Write n, e, s, and username, flush the write, then seek to top.
    ss_write_pub(n, username, file);
    fflush(file);
    fseek(file, 0, SEEK_SET);

    // Check n.
    ck_assert_msg(gmp_fscanf(file, "%Zx\n", np) == 1, "failed to scan n from public key");
    ck_assert_msg(mpz_cmp(n, np) == 0, "n written to public key doesn't match original n");

    // Check username.
    char username2[_POSIX_LOGIN_NAME_MAX];
    ck_assert_msg(fscanf(file, "%s\n", username2) == 1, "failed to scan username from public key");
    ck_assert_msg(strcmp(username, username2) == 0,
        "username written to public key doesn't match original username");

    free(username);
    mpz_clears(n, np, e, ep, s, sp, NULL);
    unlink("/tmp/test_write_pub.pub");
}
END_TEST

START_TEST(test_ss_read_pub) {
    // Open public key for reading and writing (w+).
    FILE *file = fopen("/tmp/test_read_pub.pub", "w+");

    char *n_str = "227339540862705503616796633164928975737142997203907226968556531248115531244661";
    char *username = strdup("cse13s");

    mpz_t n, np, e, ep, s, sp;
    mpz_inits(n, np, e, ep, s, sp, NULL);
    mpz_set_str(n, n_str, 10);

    // Write n, e, s, and the username, then flush file.
    ss_write_pub(n, username, file);
    fflush(file);

    // Seek to top and run student ss_read_pub().
    fseek(file, 0, SEEK_SET);
    char username2[_POSIX_LOGIN_NAME_MAX];
    ss_read_pub(np, username2, file);

    // Check n.
    ck_assert_msg(mpz_cmp(n, np) == 0, "n read in from public key doesn't match original n");

    // Check username.
    ck_assert_msg(strcmp(username, username2) == 0,
        "username read in from public key doesn't match original username");

    free(username);
    mpz_clears(n, np, e, ep, s, sp, NULL);
    unlink("/tmp/test_read_pub.pub");
}
END_TEST

START_TEST(test_ss_make_priv) {
    mpz_t p, q, pq, n, e, d, e_d, pmo, qmo, tot;
    mpz_inits(p, q, pq, n, e, d, e_d, pmo, qmo, tot, NULL);

    char *p_q[5][2] = {
        { "282667573715874331955471879035022859397946790336497", "965800189742556178908891809" },
        { "38543910776121995243183902247703732894107", "9048738936722219935073673624255354809" },
        { "1085359234116287388720478351363811140865174427907607", "191042442775945235028891433" },
        { "794931356271381704565155601817585805236889601190488133", "230182009586501290054769" },
        { "83560179265945086440620818644802074286085319", "4579445444195294997325436832806627" },
    };

    char *expected[5] = {
        "16030963153797788692470861733238667598989859238067801117832927268661895764065",
        "65705498840135473795475056535744562230406192243950205072515749054857492370049",
        "63776191833436569888643326731736944468564203472423356868747004963569817487897",
        "18058357606984441820485025795423560860375563280516157702572324193764029133113",
        "163770110675670203148266975266741248085388202181641238161403076250868737318647",
    };

    for (size_t i = 0; i < 5; i += 1) {
        mpz_set_str(p, p_q[i][0], 10);
        mpz_set_str(q, p_q[i][1], 10);


        // Compute student private key.
        ss_make_priv(d, pq, p, q);

        char *d_str = mpz_get_str(NULL, 10, d);
        char *e_d_str = expected[i];
        mpz_set_str(e_d, expected[i], 10);

        ck_assert_msg(mpz_cmp(d, e_d) == 0, "d generated incorrectly, expected: %s, got: %s\n",
            e_d_str, d_str);

        free(d_str);
    }

    mpz_clears(p, q, e, d, e_d, pmo, qmo, tot, NULL);
}
END_TEST

START_TEST(test_ss_write_priv) {
    // Open private key for reading and writing (w+).
    FILE *file = fopen("/tmp/test_write_priv.priv", "w+");

    char *n_str = "227339540862705503616796633164928975737142997203907226968556531248115531244661";
    char *d_str = "56668528966494357880294398494951543086619514460539260422492529495435274193667";

    mpz_t n, np, d, dp;
    mpz_inits(n, np, d, dp, NULL);
    mpz_set_str(n, n_str, 10);
    mpz_set_str(d, d_str, 10);

    // Write n and d to private key, flush the write, then seek back to top.
    ss_write_priv(n, d, file);
    fflush(file);
    fseek(file, 0, SEEK_SET);

    // Check n.
    ck_assert_msg(gmp_fscanf(file, "%Zx\n", np) == 1, "failed to scan n from private key");
    ck_assert_msg(mpz_cmp(n, np) == 0, "n written to private key doesn't match original n");

    // Check d.
    ck_assert_msg(gmp_fscanf(file, "%Zx\n", dp) == 1, "failed to scan d from private key");
    ck_assert_msg(mpz_cmp(d, dp) == 0, "d written to public key doesn't match original d");

    mpz_clears(n, np, d, dp, NULL);
    unlink("/tmp/test_write_priv.priv");
}
END_TEST

START_TEST(test_ss_read_priv) {
    // Open private key for reading and writing (w+).
    FILE *file = fopen("/tmp/test_read_priv.priv", "w+");

    char *n_str = "227339540862705503616796633164928975737142997203907226968556531248115531244661";
    char *d_str = "56668528966494357880294398494951543086619514460539260422492529495435274193667";

    mpz_t n, np, d, dp;
    mpz_inits(n, np, d, dp, NULL);
    mpz_set_str(n, n_str, 10);
    mpz_set_str(d, d_str, 10);

    // Write n and d, then flush the file.
    ss_write_priv(n, d, file);
    fflush(file);

    // Seek to top and run student ss_read_priv().
    fseek(file, 0, SEEK_SET);
    ss_read_priv(np, dp, file);

    // Check n.
    ck_assert_msg(mpz_cmp(n, np) == 0, "n read in from private key doesn't match original n");

    // Check d.
    ck_assert_msg(mpz_cmp(d, dp) == 0, "d read in from private key doesn't match original d");

    mpz_clears(n, np, d, dp, NULL);
    unlink("/tmp/test_read_priv.priv");
}
END_TEST

START_TEST(test_ss_encrypt) {
    mpz_t c, m, e, n, e_c;
    mpz_inits(c, m, e, n, e_c, NULL);

    char *e_n[5][2] = {
        { "92702299021870806178000378371583601020083194549210476272881391733429009719693",
            "273000396328859415542750624641630553496257682520874026912122055383148477053073" },
        { "81385966499445168096613485649898168764462512719286305131578732393591173313689",
            "348773786213442258181221261433669204535858712953263533014018928335529710210563" },
        { "2702694492298902955229270415153567508535864448502288792716328979534657688029",
            "207349679375004580722103658414326465739056577674046074636012420919850957830831" },
        { "102362675413067537644701951811450531711882508658635186619304269810804868359707",
            "182978897069869655920804543085465906273379395926307213797425229301793414556277" },
        { "109359471150712747645627468934437810640813696105298328014786794892203847483981",
            "382659282255574375440942952422138047964490414426481014919542073672231550609013" }
    };

    uint8_t data[5][6] = {
        { 0xff, 0x01, 0x02, 0x30, 0x44, 0xdd },
        { 0xff, 0x03, 0xae, 0x55, 0x4e, 0x1a },
        { 0xff, 0x04, 0xe1, 0x8d, 0x11, 0x37 },
        { 0xff, 0xcc, 0x10, 0x29, 0xf2, 0xbb },
        { 0xff, 0x40, 0x67, 0x2d, 0xa4, 0x2d },
    };

    char *expected[5]
        = { "165278642592303618292502388884851816391728404134573659745172046085152059760569",
              "180740056228665277253487425052117524585493849739082838284104916345652259601569",
              "119200979141459886597497646750480748276718091865287124044152156577950703549410",
              "39281671859706968088471453532785618815173211369480234768407085281236714291018",
              "296436535135641861665167446522980866858800878425487763236394620846294109289347" };

    for (size_t i = 0; i < 5; i += 1) {
        mpz_import(m, 6, 1, sizeof(uint8_t), 0, 0, data[i]);
        mpz_set_str(n, e_n[i][1], 10);

        ss_encrypt(c, m, n);
        mpz_set_str(e_c, expected[i], 10);

        char *c_str = mpz_get_str(NULL, 10, c);

        ck_assert_msg(mpz_cmp(c, e_c) == 0, "c generated incorrectly, expected: %s, got: %s\n",
            expected[i], c_str);

        free(c_str);
    }

    mpz_clears(c, m, e, n, e_c, NULL);
}
END_TEST

START_TEST(test_ss_decrypt) {
    mpz_t m, c, d, n, e_m;
    mpz_inits(m, c, d, n, e_m, NULL);

    char *n_c_d[5][3] = {
        { "273000396328859415542750624641630553496257682520874026912122055383148477053073",
            "210051476952615831738130163888724154308395877622513266347526668189702389569452",
            "225620983291342018971596977130961232813868469467359763193524912523548146110789" },
        { "348773786213442258181221261433669204535858712953263533014018928335529710210563",
            "54503936673907528031928067925178559142816078207963426833013612168239353454379",
            "338930122077272794039402607664110002501617624537554519050243604399361199158473" },
        { "207349679375004580722103658414326465739056577674046074636012420919850957830831",
            "183897343421957404218861856620290396535039322707087083356861354122905120179680",
            "151239275098455231971586836313550188138900884215153117703405449060624263102341" },
        { "182978897069869655920804543085465906273379395926307213797425229301793414556277",
            "113327911196383237268245991478942608806434208028778665815516095307033311537492",
            "172370901653705919669650147865433284851847416155557147228563540934264935540051" },
        { "382659282255574375440942952422138047964490414426481014919542073672231550609013",
            "135109870897103097990022222728888268022493659729863004595842155825312007417723",
            "280615089436139979783640438420196834430875208417503846072700823373940717608969" }
    };

    char *expected[5]
        = { "ff01023044dd", "ff03ae554e1a", "ff04e18d1137", "ffcc1029f2bb", "ff40672da42d" };

    for (size_t i = 0; i < 5; i += 1) {
        mpz_set_str(n, n_c_d[i][0], 10);
        mpz_set_str(c, n_c_d[i][1], 10);
        mpz_set_str(d, n_c_d[i][2], 10);
        mpz_set_str(e_m, expected[i], 16);

        ss_decrypt(m, c, d, n);
        char *m_str = mpz_get_str(NULL, 16, m);

        ck_assert_msg(mpz_cmp(m, e_m) == 0, "m generated incorrectly, expected: %s, got: %s\n",
            expected[i], m_str);

        free(m_str);
    }

    mpz_clears(c, m, d, n, e_m, NULL);
}
END_TEST

Suite *test_suite(void) {
    Suite *suite = suite_create("");

    TCase *tests = tcase_create(TESTS);
    tcase_add_checked_fixture(tests, setup, teardown);
    tcase_add_test(tests, test_ss_make_keys);
    tcase_add_test(tests, test_ss_write_pub);
    tcase_add_test(tests, test_ss_read_pub);
    tcase_add_test(tests, test_ss_make_priv);
    tcase_add_test(tests, test_ss_write_priv);
    tcase_add_test(tests, test_ss_read_priv);
    tcase_add_test(tests, test_ss_encrypt);
    tcase_add_test(tests, test_ss_decrypt);

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
