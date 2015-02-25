#include "quadrature.h"
#include <math.h>

/* Here, f1 is a specific real-valued function of a real variable */
double f1(double x)
{
  return 1.0 + pow(x, 3); /* 1 + x**3 */
}

int main(int argc, char *argv[])
{

  double a, b, int_true;
  int nvals[7], k;
  double (*fp)(double); /* fp is a function pointer */

  fp = f1; /* Assign function pointer fp as f1 */
  a = 0.0; /* Left endpoint of integration interval */
  b = 2.0; /* Right endpoint of integration interval */
  int_true = (b - a) + (pow(b, 4) - pow(a, 4)) / 4.0;
  printf("true integral: %22.14e\n\n", int_true);

  /* Fill in array of values of N for trapezoidal integration */
  for (k = 0; k < 7; k++)
    nvals[k] = 5 * pow(2, k);

  /* You have to write the function error_table in quadrature.c */
  int nrows = 7; /* Number of rows in the table (and length of nvals) */
  error_table(fp, a, b, nrows, nvals, int_true);

  return 0;
}
