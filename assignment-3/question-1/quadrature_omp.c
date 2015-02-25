#include "quadrature.h"
#include <omp.h>

double trapezoid(double (*fp)(double), double a, double b, int N)
{
    double h = (b-a)/(N-1);
    double f_a, f_b, trapezoid = 0.0;

    // Perform the trapezoid rule
    #pragma omp parallel for reduction(+: trapezoid)
    for (int i = 0; i < N; ++i)
    {
        if (i == 0)
        {
            f_a = (*fp)(a);
            trapezoid += f_a;
        }
        else if (i == N-1)
        {
            f_b = (*fp)(a + h*i);
            trapezoid += f_b;
        }
        else
        {
            trapezoid += (*fp)(a + h*i);
        }
    }
    trapezoid = h*(trapezoid) - 0.5*h*(f_a + f_b);
    return trapezoid;
}

void error_table(double (*fp)(double), double a, double b, 
                 int nrows, int nvals[], double int_true)
{
    double last_error = 0.0, error, int_trap, ratio;

    printf("%8s %22s %13s %13s\n", "n", "trapezoid", "error", "ratio");
    for (int i = 0; i < nrows; ++i)
    {
        int_trap = trapezoid(fp, a, b, nvals[i]);
        error = fabs(int_trap - int_true);
        ratio = last_error / error;
        last_error = error; // for next n
        printf("%8d %22.14e %13.3e %13.3e\n", nvals[i], int_trap, error, ratio);
    }
}
