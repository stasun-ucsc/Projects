#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathlib.h"

static int terms = -1;

double pi_bbp(void) {
	terms = 0;
	double summation = 0.0;
	// arbitrary value greater than EPSILON so program enters while loop
	double next_term = 4.0;	
	while (absolute(next_term) >= EPSILON) {
		next_term = (4.0 / (8.0 * terms + 1.0)) - (2.0 / (8.0 * terms + 4.0));
		next_term -= ((1.0 / (8.0 * terms + 5.0)) + (1.0 / (8.0 * terms + 6.0)));
		for (int i = 0; i < terms; i++) {
			next_term /= 16;
		}
		
		summation += next_term;
		terms++;
	}

	return summation;
}

int pi_bbp_terms(void) {
	if (terms < 0) {
		fprintf(stderr, "ERROR: pi_bbp() had not been previously called\n");
	}

	return terms;
}

