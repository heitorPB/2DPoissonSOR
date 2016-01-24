/**
 * @file
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief Solves a Poisson equation in 2D with Dirichlet's condition using SOR.
 *
 *
 */

#ifndef POISSONSOR2D_CUDA_H_INCLUDED
#define POISSONSOR2D_CUDA_H_INCLUDED

#include <math.h>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/iterator/zip_iterator.h>
#if CUDA_VERSION >= 6050
#include <thrust/transform_reduce.h>
#else
#include <thrust/transform.h>
#endif


/** @brief Solver of Poisson Equation.
 *
 * Solves the equation @f$ \frac{\partial^2 f}{\partial x^2} + 
 * \frac{\partial^2 f}{\partial x^2} = g(x, y) @f$ using Successive over
 * relaxation method in the unit square.
 *
 * The solution is a linear array of size N^2 indexed as 
 * f(x, y) = f[x + y * N]. This function uses the inital value of f as a guess
 * and iteratively approaches a better solution with the boundary values in
 * b.
 *
 * The point (0, 0) is at the lower left.
 *
 * The boundary condition is specified in the vector f as the exterior points.
 *
 * @return
 * * 0 on success
 * * -1 on memory error
 * * 1 on f not allocated
 */
int PoissonSOR2D_CUDA(double *f, /**< [in, out] numerical result */
                 double gamma, /**< [in] SOR parameter */
                 int N, /**< [in] number of grid points in each dimension */
                 int tmax, /**< [in] maximum number of iterations */
                 double prec /**< [in] desired precision */);


/** @brief SOR Itself. Not to be called by user.
 *
 * This function does one step of SOR. The new solution is stored in f.
 *
 * implementation according to @cite berkeley
 */
__global__
void update_CUDA(double *f,     /**< [out] new solution */
                 double *f_old, /**< [in] previous solution */
                 double gamma,  /**< [in] value of SOR parameter */
                 int N          /**< [in] grid size */);


/** @brief RHS of Poisson Equation.
 *
 * When this function is zero, we recover Laplace's Equation.
 *
 * (x,y) position has to be between (0,0) and (N-1,N-1).
 *
 * @return value of RHS at point (x,y)
 */
__device__
double g_CUDA(int x, /**< [in] x position */
	      int y, /**< [in] y position */
	      int N  /**< [in] size of grid */);


template<typename T>
class diff_thr : public thrust::unary_function<thrust::tuple<T, T>, T>
{
public:
	__host__ __device__
	T operator()(const thrust::tuple<T, T>& x) const {
		return (fabs(thrust::get<1>(x) - thrust::get<0>(x)));
	}
};


template<typename T>
T diffGPU(T *A_d, T *B_d, int N)
{
	typedef thrust::device_ptr<T> FloatIterator;
	typedef thrust::tuple<FloatIterator, FloatIterator> IteratorTuple;
	typedef thrust::zip_iterator<IteratorTuple> ZipIterator;

	thrust::device_ptr<T> A_ptr(A_d);
	thrust::device_ptr<T> B_ptr(B_d);

	ZipIterator first =
		thrust::make_zip_iterator(thrust::make_tuple(A_ptr, B_ptr));
	ZipIterator last =
		thrust::make_zip_iterator(thrust::make_tuple(A_ptr + N*N, B_ptr + N*N));

	T diff = thrust::transform_reduce(first, last, diff_thr<T>(), static_cast<T>(0), thrust::maximum<T>());

	return (diff);
}





#endif
