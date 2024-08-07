#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "mathlib.h"

#define OPTIONS "aebmrvnsh"

int main(int argc, char **argv) {
	int opt = 0;
	uint16_t flags = 0x0000;

	char *help = "SYNOPSIS\n"
   		     "   A test harness for the small numerical library.\n\n"
		     "USAGE\n"
   		     "   ./mathlib-test-amd64 [-aebmrvnsh]\n\n"
		     "OPTIONS\n"
		     "  -a   Runs all tests.\n"
		     "  -e   Runs e test.\n"
		     "  -b   Runs BBP pi test.\n"
		     "  -m   Runs Madhava pi test.\n"
		     "  -r   Runs Euler pi test.\n"
		     "  -v   Runs Viete pi test.\n"
		     "  -n   Runs Newton square root tests.\n"
		     "  -s   Print verbose statistics.\n"
		     "  -h   Display program synopsis and usage.\n";
	
	while ((opt = getopt (argc, argv, OPTIONS)) != -1) {
		switch (opt) {
		case 'h':
			flags |= 0x8000;
			break;		
		case 'a':
			flags |= 0x4000;
			break;
		case 'e':
			flags |= 0x2000;
		       	break;
		case 'b':
			flags |= 0x1000;
			break;
		case 'm':
			flags |= 0x0800;
			break;
		case 'r':
			flags |= 0x0400;
			break;
		case 'v':
			flags |= 0x0200;
			break;
		case 'n':
			flags |= 0x0100;
			break;
		case 's':
			flags |= 0x0080;
			break;
		case '?':
			flags |= 0x8000;
			break;
		}
	}		
	
	if (flags == 0 || (flags & 0x8000) != 0) {
		printf("%s", help);
		return 0;
	}

	if ((flags & 0x4000) != 0) {
		flags |= 0x7f00;
	}

	double pi;
	if ((flags & 0x2000) != 0) {
		double E = e();
		printf("e() = %.15f, M_E = %.15f, diff = %.15f\n", E, M_E, absolute(E - M_E));
		if ((flags & 0x0080) != 0) {
			printf("e() terms = %d\n", e_terms());
		}
	}

	if ((flags & 0x0400) != 0) {
		pi = pi_euler();
		printf("pi_euler() = %.15f, M_PI = %.15f, diff = %.15f\n", 
			pi, M_PI, absolute(pi - M_PI));
		if ((flags & 0x0080) != 0) {
			printf("pi_euler() terms = %d\n", pi_euler_terms());
		}
	}

	if ((flags & 0x1000) != 0) {
		pi = pi_bbp();
		printf("pi_bbp() = %.15f, M_PI = %.15f, diff = %.15f\n", 
			pi, M_PI, absolute(pi - M_PI));
		if ((flags & 0x0080) != 0) {
			printf("pi_bbp() terms = %d\n", pi_bbp_terms());
		}
	}

	if ((flags & 0x0800) != 0) {
		pi = pi_madhava();
		printf("pi_madhava() = %.15f, M_PI = %.15f, diff = %.15f\n",
			pi, M_PI, absolute(pi - M_PI));
		if ((flags & 0x0080) != 0) {
			printf("pi_madhava() terms = %d\n", pi_madhava_terms());
		}
	}

	if ((flags & 0x0200) != 0) {
		pi = pi_viete();
		printf("pi_viete() = %.15f, M_PI = %.15f, diff = %.15f\n",
			pi, M_PI, absolute(pi - M_PI));
		if ((flags & 0x0080) != 0) {
			printf("pi_viete() terms = %d\n", pi_viete_factors());
		}
	}

	if ((flags & 0x0100) != 0) {
	    double calc;
	    double root;
	    for (double i = 0.0; i <= 10.0; i += 0.1) {
	        calc = sqrt_newton(i);
	        root = sqrt(i);
	        printf("sqrt_newton(%.6f) = %.15f, sqrt(%.6f) = %.15f, diff = %.15f\n",
	    	        i, calc, i, root, absolute(calc - root));
	        if ((flags & 0x0080) != 0) {
	    	    printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
	        }
	    }
	}

	return 0;
}

