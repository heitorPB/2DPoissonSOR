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


double g(int x, int y, int N)
{
	const double x0 = N/2.;
	const double y0 = N/2.;
	const double A = 1.;
	const double sig_x = (N*N/50.);
	const double sig_y = (N*N/50.);

	return A * exp(- (x - x0) * (x - x0) / sig_x - (y - y0) * (y - y0) / sig_y);
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

	writeToFile("before", N, f, NULL);
	i = PoissonSOR2D(f, func, gamma, N, tmax, prec);
	writeToFile("after", N, f, g);

	printf("%d\n", i);

	free(f);
	return 0;
}
