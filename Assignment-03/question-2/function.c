#include <math.h>

/* f(x;\alpha) = \log(\alpha x) / \sqrt{x} 
   where \alpha is some positive parameter. */
double f (double x, void * params) {
  //double alpha = *(double *) params;
  double f = pow(cos(exp(x)), 2.0);
  return f;
}
