This directory contains code modified from an example in the GNU Scientific
Library documentation (see [Numerical Integration Examples](
http://www.gnu.org/software/gsl/manual/html_node/QAGS-adaptive-integration-with-singularities.html#QAGS-adaptive-integration-with-singularities)).

The example problem to solve is to evaluate the definite integral

$$ \int_{0}^{1} \frac{\log(\alpha x)}{\sqrt{x}}\,dx $$

when $\alpha=1$. Notice that the integrand is singular at the left end point
of the integral.

The file ``function.c`` contains the implementation of the function $f$
using a pointer to a void parameter ``params`` to pass the value of $\alpha$.
The strength of this approach is that it readily generalises to integrands
with aribtrary values of $\alpha$ and, indeed, arbitrary integrands.
