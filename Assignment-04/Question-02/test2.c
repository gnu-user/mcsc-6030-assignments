#ifdef _OPENMP
#include <omp.h>
#endif
#include <stdlib.h>
#include "quadrature.h"
#include "functions.h"
#include <time.h>

int
main (int argc, char *argv[])
{
  double a, b, int_true, t_elapsed;
  int n_vals[12], m, n_threads, f_total;
  clock_t ticks_start, ticks_end; /* used for timings */

  n_threads = 1; /* default for serial mode */
  #ifdef _OPENMP
  n_threads = 4;
  omp_set_num_threads(n_threads);
  printf ("Using %2d threads.\n", n_threads);
  #endif

  /* Allocate memory to store function counts for each thread */
  f_evals = malloc (n_threads*sizeof(*f_evals));
  k = 1.0e3; /* Define wavenumber for function f       */

  a = 0.0;   /* Left endpoint of integration interval  */
  b = 2.0;   /* Right endpoint of integration interval */
  int_true =  (b - a) + (pow (b, 4) - pow (a, 4)) / 4.0 -
              (1.0/k) * (cos(k*b) - cos(k*a));
  printf ("true integral: %22.14e\n\n", int_true);

  /* Fill in array of values of N for integration */
  int n_rows = 12; /* Number of rows in the table (and length of n_vals) */
  for (m = 0; m < n_rows; m++)
    n_vals[m] = 50 * pow (2, m);

  /* call to error_table with timing */
  ticks_start = clock();
  error_table (f, a, b, n_rows, n_vals, int_true, simpson);
  ticks_end = clock();
  t_elapsed = ((double) (ticks_end - ticks_start)) / CLOCKS_PER_SEC ;
  printf ("\nElapsed time = %f12.8 seconds\n\n", t_elapsed);

  /* print the number of function evaluations by each thread: */
  f_total = 0;
  for (m = 0; m < n_threads; m++) {
    printf ("function evals by thread %2d: %13d\n", m, f_evals[m]);
    f_total += f_evals[m];
  }
  printf ("\nTotal number of function evals: %10d\n", f_total);
  free (f_evals); 
  return 0;
}
