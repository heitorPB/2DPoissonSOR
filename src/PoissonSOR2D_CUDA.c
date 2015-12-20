/*
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief Solves a Poisson equation in 2D with Dirichlet's condition.
 *
 */


#include "PoissonSOR2D_CUDA.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int PoissonSOR2D_CUDA(double *f, double gamma,
                 int N, int tmax, double prec)
{
	double *f_tmp;
	int i, t = 0;
	double norm = prec + 42.;

	dim3 block(16, 16, 1);
	dim3 grid(ceil(N/16.), ceil(N/16), 1);

	if (NULL == f)
		return 1;

	/** @todo:
	 *   - cuda alloc : f, f_temp
	 *   - cuda copy boundary to f and f_temp
	 *   - call kernel
	 *   - kernel for diff
	 *   - copy memory back
	 *   - cuda free
	 */
	if (!(f_tmp = (double *) calloc(N * N, sizeof(double)))) {
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
		update_CUDA<<<1,N>>>(f_tmp, f, NULL, gamma, N);
		update_CUDA<<<1,N>>>(f, f_tmp, &norm, gamma, N);
		t += 2;
		if (t % 100 == 0 || norm < prec)
			printf("t, norm, prec: %4d %.9f %.9f\n", t, norm, prec);
	}

	free(f_tmp);
	return 0;
}


__global__
void update_CUDA(double *f, double *f_old,
            double *norm, double gamma, int N)
{
	int i, j, k = 0;
	double lnorm = 0;

	if (NULL != norm) {
		/* for all black grid points in the interior of the grid */
		for (j = 1; j < N - 1; j++) { /* y loop */
			for (i = 1 + k; i < N - 1; i += 2) { /* x loop */
				f[i + j * N] = f_old[i + j * N] +
				               gamma * (f_old[i-1 +  j    * N] +
				                        f_old[i+1 +  j    * N] +
				                        f_old[i   + (j-1) * N] +
				                        f_old[i   + (j+1) * N] -
				                        4. * f_old[i  + j * N] -
				                        g_CUDA(i, j, N)) / 4.;
				lnorm = fmax(lnorm, fabs(f_old[i + j * N] - f[i + j * N]));
			}
			k = (k + 1) % 2;
		}

		k = 1;
		/* for all red grid points in the interior of the grid */
		for (j = 1; j < N - 1; j++) { /* y loop */
			for (i = 1 + k; i < N - 1; i += 2) { /* x loop */
				f[i + j * N] = f_old[i + j * N] +
				               gamma * (f[i-1 +  j    * N] +
				                        f[i+1 +  j    * N] +
				                        f[i   + (j-1) * N] +
				                        f[i   + (j+1) * N] -
				                        4. * f_old[i  + j * N] -
				                        g_CUDA(i, j, N)) / 4.;
				lnorm = fmax(lnorm, fabs(f_old[i + j * N] - f[i + j * N]));
			}
			k = (k + 1) % 2;
		}
		*norm = lnorm;
	} else {
		/* for all black grid points in the interior of the grid */
		for (j = 1; j < N - 1; j++) { /* y loop */
			for (i = 1 + k; i < N - 1; i += 2) { /* x loop */
				f[i + j * N] = f_old[i + j * N] +
				               gamma * (f_old[i-1 +  j    * N] +
				                        f_old[i+1 +  j    * N] +
				                        f_old[i   + (j-1) * N] +
				                        f_old[i   + (j+1) * N] -
				                        4. * f_old[i  + j * N] -
				                        g_CUDA(i, j, N)) / 4.;
			}
			k = (k + 1) % 2;
		}

		k = 1;
		/* for all red grid points in the interior of the grid */
		for (j = 1; j < N - 1; j++) { /* y loop */
			for (i = 1 + k; i < N - 1; i += 2) { /* x loop */
				f[i + j * N] = f_old[i + j * N] +
				               gamma * (f[i-1 +  j    * N] +
				                        f[i+1 +  j    * N] +
				                        f[i   + (j-1) * N] +
				                        f[i   + (j+1) * N] -
				                        4. * f_old[i  + j * N] -
				                        g_CUDA(i, j, N)) / 4.;
			}
			k = (k + 1) % 2;
		}
	}
}


__device__
double g_CUDA(int x, int y, int N)
{
	return 0.;
}
