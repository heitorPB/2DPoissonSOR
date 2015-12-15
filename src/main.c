/**
 * @file
 * @author	Heitor Pascoal de Bittencourt <heitor.bittencourt@gmail.com>
 *
 * @brief CLI for solving a 2D Poisson equation with Dirichlet's condition.
 *
 */

#include <stdio.h>
#include "PoissonSOR2D.h"


double g(double x, double y, int N)
{
	return x+y+42;
}


int main(int argc, char *argv[])
{
	char c;
	double (*func)(double, double, int);
	func = &g;
	int i;

	i = PoissonSOR2D(NULL, func, NULL, 1., 100, 100, 0.1);

	printf("%d\n", i);

	return 0;
}
