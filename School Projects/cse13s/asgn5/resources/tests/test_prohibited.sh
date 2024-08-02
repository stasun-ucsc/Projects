#!/usr/bin/bash
GREEN=$'\e[0;32m'
RED=$'\e[0;31m'
NC=$'\e[0m'
banned=$(cat ../numtheory.c ../ss.c | grep -e "mpz_powm" -e "mpz_invert" -e "mpz_gcd" -e\
    "mpz_probab_prime" -e "mpz_lcm" -e "mpz_jacobi" | wc -l)

if [ $banned -gt 0 ]
then
    echo -e "${RED}Not prohibited functions test: failed! Using banned functions${NC}"
else
    echo -e "${GREEN}Not prohibited functions test: passed!${NC}"
fi
