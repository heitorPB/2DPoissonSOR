/**
 * @file
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief CLI for solving a 2D Poisson equation with Dirichlet's condition.
 *
 */

#include <stdio.h>
#include "PoissonSOR2D.h"


double g(double x, double y, int N)
{
	return x+y+42;
}


int main(int argc, char *argv[])
{
	char c;
	double (*func)(double, double, int) = &g;
	int i;
	int N = 128;
	int tmax = 1000;
	double prec = 0.01;
	double gamma;
	double *f = NULL;
	double *b = NULL;

	gamma = SORParamSin(N);

	printf("Simulation parameters:\n");
	printf("\tgrid size: %d x %d\n", N, N);
	printf("\ttmax: %d\n", tmax);
	printf("\tprecision: %f\n", prec);
	printf("\tgamma: %f\n", gamma);

	i = PoissonSOR2D(f, func, b, gamma, N, tmax, prec);

	printf("%d\n", i);

	return 0;
}
