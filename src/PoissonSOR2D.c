/*
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief Solves a Poisson equation in 2D with Dirichlet's condition.
 *
 */

#include "PoissonSOR2D.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int PoissonSOR2D(double *f, double (*g)(int, int, int), double gamma,
                 int N, int tmax, double prec)
{
	double *f_tmp;
	int i, t = 0;
	double norm = prec + 42.;

	/** @todo check if N % 2 == 0*/
	if (NULL == f)
		return 1;

	if (!(f_tmp = calloc(N * N, sizeof(double)))) {
		perror("Temporary array allocation error:");
		return -1;
	}
	/* copy boundary conditions */
	for (i = 0; i < N; i++) {
		f_tmp[i] = f[i]; /* y = 0 */
		f_tmp[i + (N-1) * N] = f[i + (N-1) * N]; /* y = N - 1 */
		f_tmp[0 + i * N] = f[0 + i * N]; /* x = 0 */
		f_tmp[N-1 + i * N] = f[N-1 + i * N]; /* x = N-1 */
	}

	while ((t < tmax) && (norm > prec)) {
		update(f_tmp, f, g, &norm, gamma, N);
		update(f, f_tmp, g, &norm, gamma, N);
		t += 2;
		if (t % 100 == 0 || norm < prec)
			printf("t, norm, prec: %4d %.9f %.9f\n", t, norm, prec);
	}

	free(f_tmp);
	return 0;
}

void update(double *f, double *f_old, double (*g)(int, int, int),
            double *norm, double gamma, int N)
{
	int i, j;

	/* for all black grid points in the interior of the grid */
	for (j = 1; j < N - 1; j++) { /* y loop */
		for (i = 1; i < N - 1; i += 2) { /* x loop */
			f[i + j * N] = f_old[i + j * N] +
			               gamma * (f_old[i-1 +  j    * N] +
			                        f_old[i+1 +  j    * N] +
			                        f_old[i   + (j-1) * N] +
			                        f_old[i   + (j+1) * N] -
			                        4. * f_old[i  + j * N] -
			                        g(i, j, N)/(N*N)) / 4.;
		}
	}

	/* for all red grid points in the interior of the grid */
	for (j = 1; j < N - 1; j++) { /* y loop */
		for (i = 1; i < N - 1; i += 2) { /* x loop */
			f[i + j * N] = f_old[i + j * N] +
			               gamma * (f[i-1 +  j    * N] +
			                        f[i+1 +  j    * N] +
			                        f[i   + (j-1) * N] +
			                        f[i   + (j+1) * N] -
			                        4. * f_old[i  + j * N] -
			                        g(i, j, N)/(N*N)) / 4.;
		}
	}

	(*norm) = 0;
	for (j = 1; j < N - 1; j++)
		for (i = 1; i < N - 1; i++)
			(*norm) = fmax((*norm), fabs(f_old[i + j * N] - f[i + j * N]));
}
