#include <omp.h>
#include <math.h>

int *f_evals;       /* Pointer for array to store # evaluations of f */
double k;           /* Wavenumber in sin(k*x) term of function f     */
double f (double);
double f2 (double);
