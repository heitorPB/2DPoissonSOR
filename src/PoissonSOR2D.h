/**
 * @file
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief Solves a Poisson equation in 2D with Dirichlet's condition using SOR.
 *
 *
 */

#ifndef POISSONSOR2D_H_INCLUDED
#define POISSONSOR2D_H_INCLUDED

#include <math.h>

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
int PoissonSOR2D(double *f, /**< [in, out] numerical result */
                 double (*g)(int, int, int), /**< [in] RHS of Poisson Eq */
                 double gamma, /**< [in] SOR parameter */
                 int N, /**< [in] number of grid points in each dimension */
                 int tmax, /**< [in] maximum number of iterations */
                 double prec /**< [in] desired precision */);


/** @brief Get optimal parameter for SOR.
 *
 * According to @cite Yang2009325, the optimal SOR parameter is
 * @f[ \omega_{opt} = \frac{2}{1 + \sin(\pi h)} @f]
 * with @f[ h = \frac{1}{N + 1} @f]
 *
 * @return SOR optimal parameter
 */
static inline double SORParamSin(int N /**< [in] grid size along one dimension */)
{
	return (2. / (1. + sin(M_PI / (N + 1.)))) ;
}


/** @brief SOR Itself. Not to be called by user.
 *
 * This function does one step of SOR. The new solution is stored in f.
 */
void update(double *f, double *f_old, double (*g)(int, int, int),
            double *norm, double gamma, int N);
#endif
