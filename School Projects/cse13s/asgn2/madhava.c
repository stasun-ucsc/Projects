#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathlib.h"

static int terms = -1;

double pi_madhava(void) {
	terms = 0;
	double summation = 0.0;
	double next_term = 0.0;
	double pi = 0.0;
	while (absolute(M_PI - pi) >= EPSILON) {
		next_term = 1.0 / (2.0 * terms + 1.0);
		for (int i = 0; i < terms; i++) {	
			next_term /= -3.0;
		}

		summation += next_term;
		pi = sqrt_newton(12) * summation;
		terms++;
	}

	return pi;
}

int pi_madhava_terms(void) {
	if (terms < 0) {
		fprintf(stderr, "ERROR: pi_madhava() had not been previously called\n");
	}	

	return terms;
}

