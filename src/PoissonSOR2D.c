/**
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief Solves a Poisson equation in 2D with Dirichlet's condition.
 *
 *
 */

#include "PoissonSOR2D.h"


int PoissonSOR2D(double *f, double (*g)(double, double, int), double *b,
                 double gamma, int N, int tmax, double prec)
{
	return (int) (*g)(prec, tmax, N);
}
