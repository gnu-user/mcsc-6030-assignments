#include "quadrature.h"

double trapezoid (double (*fp)(double), double a, double b, int N)
{
  /*
     Estimate the integral of f(x) from a to b using the
     Trapezoid Rule with n points.

     Input:
       f:  the function to integrate
       a:  left endpoint
       b:  right endpoint
       N:  number of points to use
     Returns:
       numerical estimate of the integral
  */
  double h = (b-a)/(N-1), total = 0.0;
  #pragma omp parallel for reduction(+ : total)
  for (int m=0; m<N; m++) {
     double x = m*h + a;
     total += fp(x);
  }
  total -= 0.5*(fp(a)+fp(b));
  return h*total;
}

double simpson (double (*fp)(double), double a, double b, int N)
{
  /*
     Estimate the integral of f(x) from a to b using Simpson's Rule 
     with n points.

     Input:
       f:  the function to integrate
       a:  left endpoint
       b:  right endpoint
       N:  number of points to use
     Returns:
       numerical estimate of the integral
  */
  double h = (b-a)/(N-1), total = 0.0;

  #pragma omp parallel for reduction(+ : total)
  for (int m = 0; m < N; m++) {
     double xj = m*h + a;
     double xc = xj + h/2.0;
     total += 2.0*fp(xj) + 4.0*fp(xc);
  }
  
  /* Remove the extra values from the total */
  total -= fp(a) + fp(b) + 4.0*fp(b + h/2.0);
  return (h/6.0)*total;
}

void error_table (double (*fp)(double), double a, double b, 
                  int nrows, int nvals[], double int_true,
                  double (*method)(double (*)(double), double, double, int))
{
  double ratio, last = 1.0;
  printf ("%8s %22s %13s %13s\n", "n", "trapezoid", "error", "ratio");
  for (int m=0;m<nrows;m++) {
    double val = method ( fp, a, b, nvals[m] );
    double err = fabs(val - int_true);
    ratio = last/err;
    last = err;
    printf("%8d %22.14e %13.3e %13.3e\n", nvals[m], val, err, ratio);
  }
  return;
}
