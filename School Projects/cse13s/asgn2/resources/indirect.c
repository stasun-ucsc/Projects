// ☠️  2021 Darrell Long ☠️

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define EPSILON 1e-17
#define M_PI    3.141592653589793

double fabs(double x) { return x < 0 ? -x : x; }

// Private: Floating point modulus: a % b

static inline double modulus(double a, double b) { return a - b * (int64_t)(a / b); }

// Square-root using Newton's algorithm using the iterate:
//
//     y + x / y
// y = ---------
//         2

double sqrt(double x) {
  assert(x >= 0);
  double f = 1.0, // Scaling factor
         y = 1.0; // Initial guess
  while (x > 4) { // Normalize to [0, 4) by reducing the exponent in IEEE 754
    x /= 4.0;     // Every 4 going in results in 2 going out since √4 = 2
    f *= 2.0;     // This should not disturb the mantissa
  }
  for (double guess = 0.0; fabs(y - guess) > EPSILON; y = (y + x / y) / 2.0) {
    guess = y;
  }
  return f * y; // Rescale back to the original range
}

// Expand the Taylor series:
//
//               3    5      7       9        11          13             15
//              x    x      x       x        x           x              x              16
// sin(x) = x - -- + --- - ---- + ------ - -------- + ---------- - ------------- + O(x)
//              3!    5!    7!      9!       11!         13!            15!

double sin(double x) {
  x = modulus(x, 2 * M_PI); // Normalize to [-2π, 2π]
  double val = x, trm = x;
  for (double k = 3.0; fabs(trm) > EPSILON; k += 2.0) {
    val += trm *= -(x * x) / ((k - 1) * k);
  }
  return val;
}

// Expand the Taylor series:
//
//               2    4    6      8        10         12           14
//              x    x    x      x        x          x            x             16
// cos(x) = 1 - -- + -- - --- + ----- - ------- + --------- - ----------- + O(x)
//              2    4!   6!     8!       10!        12!          14!

double cos(double x) {
  x = modulus(x, 2 * M_PI); // Normalize to [-2π, 2π]
  double val = 1, trm = 1;
  for (double k = 1.0; fabs(trm) > EPSILON; k += 2.0) {
    val += trm *= -(x * x) / ((k + 1) * k);
  }
  return val;
}

double asin(double);

double acos(double);

// Calculate arcsin(x) as the root of sin(x) - a = 0 using the iterate:
//
// x - sec(x) (sin(x) - a)
//
// When x ≈ 1, we want to take a different approach and rely on
// trigometric identities.

double asin(double a) {
  assert(a >= -1 && a <= 1);
  double x = 0;
  if (fabs(a) > 31.0 / 32.0) {
    return a < 0 ? -acos(sqrt(1 - a * a)) : acos(sqrt(1 - a * a));
  } else {
    while (fabs(sin(x) - a) > EPSILON) {
      x = x - (sin(x) - a) / cos(x);
    }
    return x;
  }
}

// Calculate arcos(x) as the root of cos(x) - a = 0 using the iterate:
//
// x + (cos(x) - a) csc(x)
//
// When x ≈ 1, we want to take a different approach and rely on
// trigometric identities.

double acos(double a) {
  assert(a >= -1 && a <= 1);
  double x = 1.0;
  if (fabs(a) > 31.0 / 32.0) {
    return a < 0 ? M_PI - asin(sqrt(1 - a * a)) : asin(sqrt(1 - a * a));
  } else {
    while (fabs(cos(x) - a) > EPSILON / 10) {
      x = x + (cos(x) - a) / sin(x);
    }
    return x;
  }
}

double atan(double);

double acot(double);

//               ⎡     1      ⎤
// acot(x) = asin⎢------------⎥, |x| < 1 and atan(1/x), |x| ≥ 1
//               ⎢          2 ⎥
//               ⎣sqrt(1 + x )⎦
//
// When x ≥ 1, we want to take a different approach and rely on trigometric
// identities.

double acot(double x) {
  if (x > 1) {
    return atan(1 / x);
  } else if (x < -1) {
    return -atan(1 / x);
  } else {
    return asin(1 / sqrt(1 + x * x));
  }
}

//               ⎡     x      ⎤
// atan(x) = asin⎢------------⎥, |x| < 1 and acot(1/x), |x| ≥ 1
//               ⎢          2 ⎥
//               ⎣sqrt(1 + x )⎦
//
// When x ≥ 1, we want to take a different approach and rely on trigometric
// identities.

double atan(double x) {
  if (x > 1) {
    return acot(1 / x);
  } else if (x < -1) {
    return -acot(1 / x);
  } else {
    return asin(x / sqrt(1 + x * x));
  }
}

int main(void) {
    printf("The hard way...\n");
    printf("π ≈ %33.32lf\n", 4 * atan(1.0));
    return 0;
}
