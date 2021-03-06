#include <stdio.h>
#include <math.h>

double trapezoid (double (*fp)(double), double a, double b, int N);
double simpson (double (*fp)(double), double a, double b, int N);
void error_table (double (*fp)(double), double a, double b, 
                  int nrows, int nvals[], double int_true,
                  double (*method)(double (*)(double), double, double, int));
