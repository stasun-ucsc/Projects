#include <stdio.h>
#include <stdlib.h>
#include "mathlib.h"

static int iters = -1;

double sqrt_newton(double x) {
	iters = 0;
	double prev_term = 0.0;
	double approx_root = 1.0;
	while (absolute(approx_root - prev_term) >= EPSILON) {
		prev_term = approx_root;
		approx_root = 0.5 * (prev_term + x / prev_term);
		iters++;
	}
	
	return approx_root;
}

int sqrt_newton_iters(void) {
	if (iters < 0) {
		fprintf(stderr, "ERROR: sqrt_newton() had not been previously called\n");
        	exit(EXIT_FAILURE);
	}	

	return iters;
}

