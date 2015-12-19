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
	$ make
	$ ./2DSOR

Default is to run with grid size 128x128, maximum of 1000 iterations.
3 files will be created:

- before_l.sol
- after_l.sol
- after_l.g

This files contain the values of f before the algorithm (with boundary 
conditions), the numerical solution (file after.sol) and the function
g(x, y) (after.g).
They are formated to be used with the Python script src/plotter.py:

	$ python3 plotter.py after.sol

The result is:

@image html Laplace.sol.png "Solution to Laplace's Equation" width=\\textwidth

Gaussian
--------

We use a 2D gaussian function to validate the code:

@f[ 
  \left( \frac{\partial}{\partial x} + \frac{\partial}{\partial y} \right)
  f(x, y) = 
  g(x, y) = A e^{\left(- \left(\frac{(x-x_0)^2}{2\sigma_x^2} + \frac{(y-y_0)^2}{2\sigma_y^2} \right)\right)}
@f]

We set @f$ A = 1 @f$, @f$ 2 \sigma_{x,y} = 1 @f$, @f$ x_0 = y_0 = N / 2 @f$.

We know the solution in this case:

@f[
  f(x, y) = 4 e^{\left(- ((x-x_0)^2 + (y - y_0)^2) \right)} (x^2 + y^2 - 1)
@f]


