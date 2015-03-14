#include "functions.h"

double f(double x) {
  int thread_num; 
  /* track of number of function evaluations by each thread: */
  thread_num = 0;   /* default for serial mode */
#ifdef _OPENMP
  thread_num = omp_get_thread_num();
#endif
  f_evals[thread_num]++;
  return (1.0 + x*x*x + sin(k*x));
}
