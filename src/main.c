/**
 * @file
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief CLI for solving a 2D Poisson equation with Dirichlet's condition.
 *
 */

#include <stdio.h>
#include "PoissonSOR2D.h"
#include <stdlib.h>


double g(int x, int y, int N)
{
	if ((x - N/2. < 0.1) && (y - N/2. < 0.1))
		return 1.;
	return 0;
}


int main(int argc, char *argv[])
{
	char c;
	double (*func)(int, int, int) = &g;
	int i;
	int N = 128;
	int tmax = 1000;
	double prec = 0.1e-5;
	double gamma;
	double *f = NULL;
	double *b = NULL;

	gamma = SORParamSin(N);

	printf("Simulation parameters:\n");
	printf("\tgrid size: %d x %d\n", N, N);
	printf("\ttmax: %d\n", tmax);
	printf("\tprecision: %f\n", prec);
	printf("\tgamma: %f\n", gamma);

	if (!(f = calloc(N*N, sizeof(double)))) {
		perror("Memory allocation problem: ");
		return 1;
	}

	i = PoissonSOR2D(f, func, gamma, N, tmax, prec);

	printf("%d\n", i);

	free(f);
	return 0;
}
