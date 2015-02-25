#include "quadrature.h"

double trapezoid(double (*fp)(double), double a, double b, int N)
{
    double h = (b-a)/(N-1);
    double f_a, f_b, trapezoid = 0.0;

    // Perform the trapezoid rule
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
    return;
}
