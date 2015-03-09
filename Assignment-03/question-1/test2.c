#include "quadrature.h"
#include <math.h>

// Define K constant explicitly, similar to python
#define K 1000

/* Here, f1 is a specific real-valued function of a real variable */
double f1(double x)
{
  return 1.0 + pow(x, 3) + sin(K*x); /* 1 + x**3 + sin(k*x) */
}

int main(int argc, char *argv[])
{

  double a, b, int_true;
  int nvals[12], nrows = 12, k;
  double (*fp)(double); /* fp is a function pointer */

  fp = f1; /* Assign function pointer fp as f1 */
  a = 0.0; /* Left endpoint of integration interval */
  b = 2.0; /* Right endpoint of integration interval */
  int_true = (b - a) + (pow(b, 4) - pow(a, 5)) / 4.0 - (1.0/K) * (cos(K*b) - cos(K*a));
  printf("true integral: %22.14e\n\n", int_true);

  /* Fill in array of values of N for trapezoidal integration */
  for (k = 0; k < 12; k++)
    nvals[k] = 5 * pow(2, k);

  error_table(fp, a, b, nrows, nvals, int_true);

  return 0;
}
