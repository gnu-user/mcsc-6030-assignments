#include <stdio.h>
#include <gsl/gsl_integration.h>
#include "function.h"

extern double f (double, void*);

int
main (void)
{
  /* Claim memory needed for adaptive integration */
  gsl_integration_workspace * w 
    = gsl_integration_workspace_alloc (1000);
  
  /* Construct GSL function object to describe solution */
  gsl_function F;
  F.function = &f;
  double alpha = 1.0;
  F.params = NULL;

  /* Parameters to control quadrature routine */
  double a, b, tol_abs, tol_rel;
  int limit;
  a = 0.0;
  b = 3.0;
  tol_abs = 0;
  tol_rel = 1.0e-7;
  limit = 1000;

  // Test that the function is implemented properly
  //double test = GSL_FN_EVAL(&F,2);
  //printf("TEST RESULT: %.18f\n", test);


  /* Actual call to quadrature routine */
  double result, error;
  gsl_integration_qag (&F, a, b, tol_abs, tol_rel, limit,
                        GSL_INTEG_GAUSS61, w, &result, &error); 
  double mathematica_value = 1.29646778572437307899;

  printf ("result                = % .18f\n", result);
  printf ("exact result          = % .18f\n", exact_value);
  printf ("estimated error       = % .18f\n", error);
  printf ("error vs. mathematica = % .18f\n", 
           fabs(result - mathematica_value));
  printf ("# intervals     = %d\n", (int) w->size);

  /* Return memory allocated for solver */
  gsl_integration_workspace_free (w);

  return 0;
}
