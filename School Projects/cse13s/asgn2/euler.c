#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathlib.h"

static long terms = -1;

double pi_euler(void) {
	terms = 0;
	double pi = 0.0;
	double summation = 0.0;
	double new_term;
	while (terms < 10000000 && (absolute(M_PI - pi) >= EPSILON)) {
		terms++;
		new_term = 1.0 / (terms * terms);
		summation += new_term;
		pi = sqrt_newton(6 * summation);
	}

	return pi;
}

int pi_euler_terms(void) {
	if (terms < 0) {
		fprintf(stderr, "ERROR: pi_euler() had not been previously called\n");
		exit(EXIT_FAILURE);
	}

	return terms;
}

