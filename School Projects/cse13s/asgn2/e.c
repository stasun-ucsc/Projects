#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathlib.h"

static int terms = -1;

double e(void) {
    terms = 1;
    double prev_term = 1;
    double e = prev_term;
    while (absolute(M_E - e) >= EPSILON) {
    	prev_term *= (1.0/terms);
	e += prev_term;
	terms++;
    }

    return e;
}

int e_terms(void) {
    if (terms < 0) {
	fprintf(stderr, "ERROR: e() had not been previously called\n");
    	exit(EXIT_FAILURE);
    }

    return terms;
}

