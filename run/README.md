## Run examples


Laplace's Equation
------------------

When we set @f$ g(x,y) = 0 @f$ we get Laplace's Equation:

@f[
  \left( \frac{\partial}{\partial x} + \frac{\partial}{\partial y} \right)
  f(x, y) = 0
@f]

Together with the boundary conditions:

\f{eqnarray*}{
	f(x, y = 0) &=& -(x - x_0)^2 / x_0^2 + 1 \\ 
	f(x, y = 1) &=& 0 \\
	f(x = 0, y) &=& 0 \\ 
	f(x = 1, y) &=& 0
\f}

The corresponding code is in file @ref run/Laplace.c in this folder.

To compile this example, copy this Laplace to src folder with name main.c,
then compile and run:

	$ cp run/Laplace.c src/main.c
	$ cd src
	$ make
	$ ./2DSOR

Default is to run with grid size 128x128, maximum of 1000 iterations.
1 file will be created:

- solution.sol

This file contains the numerical solution formated to be used with the Python
script src/plotter.py:

	$ python3 plotter.py solution.sol

The result is:

@image html Laplace.sol.png "Solution to Laplace's Equation" width=\\textwidth

