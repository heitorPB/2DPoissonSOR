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
#ifdef _OPENMP
#include <omp.h>
#endif


int PoissonSOR2D(double *f, double (*g)(int, int, int), double gamma,
                 int N, int tmax, double prec)
{
	double *f_tmp;
	int i, t = 0;
	double norm = prec + 42.;
	#ifdef _OPENMP
	const int chunk = ceil(N / omp_get_max_threads());
	#endif

	if (NULL == f)
		return 1;

	if (!(f_tmp = (double *) calloc(N * N, sizeof(double)))) {
		perror("Temporary array allocation error:");
		return -1;
	}

	/* copy boundary conditions */
	#pragma omp parallel for schedule(static, chunk)
	for (i = 0; i < N; i++) {
		f_tmp[i] = f[i]; /* y = 0 */
		f_tmp[i + (N-1) * N] = f[i + (N-1) * N]; /* y = N - 1 */
		f_tmp[0 + i * N] = f[0 + i * N]; /* x = 0 */
		f_tmp[N-1 + i * N] = f[N-1 + i * N]; /* x = N-1 */
	}

	while ((t < tmax) && (norm > prec)) {
		update(f_tmp, f, g, NULL, gamma, N);
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
	int i = 1, j, k = 0;
	double lnorm = 0;
	#ifdef _OPENMP
	const int chunk = ceil(N / omp_get_max_threads());
	#endif

	if (NULL != norm) {
		/* for all black grid points in the interior of the grid */
		#pragma omp parallel for reduction(max:lnorm) private(i,j,k) shared(f,f_old) schedule(static,chunk)
		for (j = 1; j < N - 1; j++) { /* y loop */
			k = (j+1) % 2;
			for (i = 1 + k; i < N - 1; i += 2) { /* x loop */
				f[i + j * N] = f_old[i + j * N] +
				               gamma * (f_old[i-1 +  j    * N] +
				                        f_old[i+1 +  j    * N] +
				                        f_old[i   + (j-1) * N] +
				                        f_old[i   + (j+1) * N] -
				                        4. * f_old[i  + j * N] -
				                        g(i, j, N)/N/N) / 4.;
				lnorm = fmax(lnorm, fabs(f_old[i + j * N] - f[i + j * N]));
			}
		}

		/* for all red grid points in the interior of the grid */
		#pragma omp parallel for reduction(max:lnorm) private(i,j,k) shared(f,f_old) schedule(static,chunk)
		for (j = 1; j < N - 1; j++) { /* y loop */
			k = (j) % 2;
			for (i = 1 + k; i < N - 1; i += 2) { /* x loop */
				f[i + j * N] = f_old[i + j * N] +
				               gamma * (f[i-1 +  j    * N] +
				                        f[i+1 +  j    * N] +
				                        f[i   + (j-1) * N] +
				                        f[i   + (j+1) * N] -
				                        4. * f_old[i  + j * N] -
				                        g(i, j, N)/N/N) / 4.;
				lnorm = fmax(lnorm, fabs(f_old[i + j * N] - f[i + j * N]));
			}
		}
		*norm = lnorm;
	} else {
		/* for all black grid points in the interior of the grid */
		#pragma omp parallel for private(i,j,k) shared(f,f_old) schedule(static,chunk)
		for (j = 1; j < N - 1; j++) { /* y loop */
			k = (j + 1) % 2;
			for (i = 1 + k; i < N - 1; i += 2) { /* x loop */
				f[i + j * N] = f_old[i + j * N] +
				               gamma * (f_old[i-1 +  j    * N] +
				                        f_old[i+1 +  j    * N] +
				                        f_old[i   + (j-1) * N] +
				                        f_old[i   + (j+1) * N] -
				                        4. * f_old[i  + j * N] -
				                        g(i, j, N)/N/N) / 4.;
			}
		}

		/* for all red grid points in the interior of the grid */
		#pragma omp parallel for private(i,j,k) shared(f,f_old) schedule(static,chunk)
		for (j = 1; j < N - 1; j++) { /* y loop */
			k = (j) % 2;
			for (i = 1 + k; i < N - 1; i += 2) { /* x loop */
				f[i + j * N] = f_old[i + j * N] +
				               gamma * (f[i-1 +  j    * N] +
				                        f[i+1 +  j    * N] +
				                        f[i   + (j-1) * N] +
				                        f[i   + (j+1) * N] -
				                        4. * f_old[i  + j * N] -
				                        g(i, j, N)/N/N) / 4.;
			}
		}
	}
}


int writeToFile(const char *fname, int N, double *f, double (*g)(int, int, int))
{
	int i, j;
	FILE *fp = NULL;
	char filen[256];

	snprintf(filen, sizeof(filen), "%s%s", fname, ".sol");

	if (!(fp = fopen(filen, "w"))) {
		perror("Unable to write files");
		return -1;
	}

	/* write some header */
	fprintf(fp, "# N = %d \n", N);

	for (j = 0; j < N; j++) {
		fprintf(fp, "  ");
		for (i = 0; i < N; i++)
			fprintf(fp, "%4.8f\t ", f[i + j * N]);
		fprintf(fp, "\n");
	}

	fclose(fp);

	if (NULL != g) {
		snprintf(filen, sizeof(filen), "%s%s", fname, ".g");

		if (!(fp = fopen(filen, "w"))) {
			perror("Unable to write files");
			return -1;
		}

		fprintf(fp, "# N = %d \n", N);

		for (j = 0; j < N; j++) {
			fprintf(fp, "  ");
			for (i = 0; i < N; i++)
				fprintf(fp, "%4.8f\t ", g(i, j, N));
			fprintf(fp, "\n");
		}

		fclose(fp);
	}

	return 0;
}
