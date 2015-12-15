/**
 * @file
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief Solves a Poisson equation in 2D with Dirichlet's condition using SOR.
 *
 *
 */


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
 * The boundary is specified as a linear vector b, indexed the following way:
 *  * first N-1 elements correspond to (x, y = 0)
 *  * elements b[N:2N - 1] correspond to (x = N - 1, y)
 *  * elements b[2N:3N - 1] correspond to (x, y = N - 1)
 *  * elements b[3N:4N - 1] correspond to (x = 0, y)
 * In a counter clockwise fashion.
 */
int PoissonSOR2D(double *f, /**< [in, out] numerical result */
                 double (*g)(double, double, int), /**< RHS of Poisson Eq */
		 double *b, /**< [in] boundary values */
                 double gamma, /**< [in] SOR parameter */
                 int N, /**< [in] number of grid points in each dimension */
                 int tmax, /** [in] maximum number of iterations */
                 double prec /** [in] desired precision */);
