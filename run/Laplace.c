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
#include <unistd.h>


/** @brief RHS of Poisson Equation.
 *
 * When this function is zero, we recover Laplace's Equation.
 *
 * @return 0
 */
double g(int x, int y, int N)
{
	return 0.;
}


int main(int argc, char *argv[])
{
	char c;
	double (*func)(int, int, int) = &g;
	int i;
	int N = 128;
	int tmax = 4200;
	double prec = 0.1e-5;
	double gamma;
	double *f = NULL;
	double *b = NULL;

	/* this SOR Parameter function is weird */
	gamma = SORParamSin(N) / 10.;

	/* Parse command line*/
	while ((c = getopt(argc, argv, "N:t:p:g:h")) >= 0) {
		switch (c) {
		case 'N':
			N = (unsigned int) atoi(optarg);
			break;

		case 't':
			tmax = (unsigned int) atoi(optarg);
			break;

		case 'p':
			prec = atof(optarg);
			break;

		case 'g':
			gamma = atof(optarg);
			if ((gamma < 0) || (gamma > 2))
				fprintf(stdout, "Weird value of SOR parameter."
				        "Be carefull.\n%s\n", optarg);
			break;

		case '?':
		case 'h':
			fprintf(stderr, "Usage: %s [option]...\n"
				"Options:\n"
				"\t-N\tgrid size in each dimension\n"
				"\t-t\tmax number of iterations\n"
				"\t-p\tdesired precision\n"
				"\t-g\tdesired precision\n"
				"\t-h\tthis text\n",
				argv[0]);
			return 0;
		}
	}

	printf("Simulation parameters:\n");
	printf("\tgrid size: %d x %d\n", N, N);
	printf("\ttmax: %d\n", tmax);
	printf("\tprecision: %f\n", prec);
	printf("\tgamma: %f\n", gamma);

	if (!(f = calloc(N*N, sizeof(double)))) {
		perror("Memory allocation problem: ");
		return 1;
	}

	/* set boundary conditions */
	/* y = 0: f = -x^2 */
	double x0 = N/2.;
	for (i = 0; i < N; i++)
		f[i] = -(i - x0)*(i - x0) / (x0)/(x0) + 1.;

	writeToFile("before_g", N, f, NULL);
	i = PoissonSOR2D(f, func, gamma, N, tmax, prec);
	writeToFile("after_g", N, f, g);

	printf("%d\n", i);

	free(f);
	return 0;
}
