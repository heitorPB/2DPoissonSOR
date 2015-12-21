/*
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief Solves a Poisson equation in 2D with Dirichlet's condition.
 *
 */


#include "PoissonSOR2D_CUDA.h"
#include "error_checks.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int PoissonSOR2D_CUDA(double *f, double gamma,
                 int N, int tmax, double prec)
{
	double *f_tmp, *f_gpu;
	int t = 0;
	double norm = prec + 42.;

	const int blocksize = 16;
	const int gridsize = (N + blocksize - 1) / blocksize;
	dim3 block(blocksize, blocksize, 1);
	dim3 grid(gridsize, gridsize, 1);

	if (NULL == f)
		return 1;

	/** @todo kernel for diff */

	size_t size = N * N * sizeof(double);

	CUDA_CHECK(cudaMalloc((void**) &f_gpu, size));
	CHECK_ERROR_MSG("cudaMalloc");
	CUDA_CHECK(cudaMalloc((void**) &f_tmp, size));
	CHECK_ERROR_MSG("cudaMalloc_tmp");

	/* copy boundary conditions */
	CUDA_CHECK(cudaMemcpy((void*) f_gpu, (void*) f,     size, cudaMemcpyHostToDevice));
	CUDA_CHECK(cudaMemcpy((void*) f_tmp, (void*) f_gpu, size, cudaMemcpyDeviceToDevice));

	CHECK_ERROR_MSG("cudaMemcpy");

	printf("gridsize: %d x %d x %d\n", grid.x, grid.y, grid.z);
	printf("blocksize: %d x %d x %d\n", block.x, block.y, block.z);

	while ((t < tmax) && (norm > prec)) {
		update_CUDA<<<grid, block>>>(f_tmp, f_gpu, gamma, N);
		update_CUDA<<<grid, block>>>(f_gpu, f_tmp, gamma, N);

		t += 2;
		if (t % 100 == 0 || norm < prec)
			printf("t, norm, prec: %4d %.9f %.9f\n", t, norm, prec);
	}

	CHECK_ERROR_MSG("Kernel call");

	CUDA_CHECK(cudaMemcpy((void*) f, (void*) f_gpu, size, cudaMemcpyDeviceToHost));

	CHECK_ERROR_MSG("Copy memory back");

	cudaFree(f_gpu);
	cudaFree(f_tmp);
	return 0;
}


__global__
void update_CUDA(double *f, double *f_old,
                 double gamma, int N)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;

	/* if not boundary */
	if ((i > 0) && (j > 0) && (i < N-1) && (j < N-1)) {
		/* for all black points */
		if ((i + j) % 2 == 0) {
			f[i + j * N] = f_old[i + j * N] +
			               gamma * (f_old[i-1 +  j    * N] +
			                        f_old[i+1 +  j    * N] +
			                        f_old[i   + (j-1) * N] +
			                        f_old[i   + (j+1) * N] -
			                        4. * f_old[i  + j * N] -
			                        g_CUDA(i, j, N)/N/N) / 4.;
		} else {
			__syncthreads();
			/* for all red points */
			f[i + j * N] = f_old[i + j * N] +
			               gamma * (f[i-1 +  j    * N] +
			                        f[i+1 +  j    * N] +
			                        f[i   + (j-1) * N] +
			                        f[i   + (j+1) * N] -
			                        4. * f_old[i  + j * N] -
			                        g_CUDA(i, j, N)/N/N) / 4.;
		}
	}
}


__device__
double g_CUDA(int x, int y, int N)
{
	return 0.;
}
