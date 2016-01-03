/**
 * @file
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief CLI for solving a 2D Poisson equation with Dirichlet's condition.
 *
 */

#include <stdio.h>
#include "PoissonSOR2D.h"
#include "PoissonSOR2D_CUDA.h"
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

/** @brief RHS of Poisson Equation.
 *
 * When this function is zero, we recover Laplace's Equation.
 *
 * (x,y) position has to be between (0,0) and (N-1,N-1).
 *
 * @return value of RHS at point (x,y)
 */
double g(int x, /**< [in] x position */
	 int y, /**< [in] y position */
	 int N  /**< [in] size of grid */)
{
	return 0.;
}


/** @brief Main function.
 *
 * Interface for command line and calling solvers for Poisson Equation.
 */
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

	struct timespec t0, t1;
	double serial_time;
	double gpu_time;

	double *f_gpu = NULL;

	/* this SOR Parameter function is weird */
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

	if (!(f = (double*) calloc(N*N, sizeof(double)))) {
		perror("Memory allocation problem: ");
		return 1;
	}
	if (!(f_gpu = (double*) calloc(N*N, sizeof(double)))) {
		perror("Memory allocation problem: ");
		free(f);
		return 1;
	}

	/* set boundary conditions */
	/* x = 0: f = -y^2 */
	double x0 = N/2.;
	for (i = 0; i < N; i++) {
		f[i*N] = -(i - x0)*(i - x0) / (x0)/(x0) + 1.;
		f_gpu[i*N] = f[i*N];
	}

	/* run in CPU and measure time*/

	clock_gettime(CLOCK_REALTIME, &t0);
	i = PoissonSOR2D(f, func, gamma, N, tmax, prec);
	clock_gettime(CLOCK_REALTIME, &t1);

	writeToFile("cpu", N, f, NULL);

	serial_time = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1.E9;

	/* now in GPU*/

	clock_gettime(CLOCK_REALTIME, &t0);
	i = PoissonSOR2D_CUDA(f_gpu, gamma, N, tmax, prec);
	clock_gettime(CLOCK_REALTIME, &t1);

	writeToFile("gpu", N, f, NULL);

	gpu_time = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1.E9;

	printf("CPU_time: %f \t GPU_time: %f \n", serial_time, gpu_time);

	free(f);
	free(f_gpu);
	return 0;
}
