#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathlib.h"

static int factors = -1;

double pi_viete(void) {
	factors = 1;
	double prev_factor = sqrt_newton(2);
	double next_term = 0.0;
	double product = prev_factor / 2;
	double pi = 0.0;

	while (absolute(M_PI - pi) >= EPSILON) {
		factors++;
		prev_factor = sqrt_newton(2 + prev_factor);
		next_term = prev_factor / 2;
		product *= next_term;
		pi = 2 / product;
	}

	return pi;
}

int pi_viete_factors(void) {
	if (factors < 0) {
		fprintf(stderr, "ERROR: pi_viete() had not been previously called\n");
		exit(EXIT_FAILURE);
	}

	return factors;
}

