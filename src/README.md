Source code
===========

Structure of code
-----------------

### main

The file main.c contains the command line interface to run the code.

### PoissonSOR2D

Implementation of the algorithm is in PoissonSOR2D.c. Header file
PoissonSOR2D.h should be included to run the code.

### plotter

PoissonSOR2D.h contains a function to write data do a file. The Python
script plotter.py plots this output file:

	$ python3 plotter.py outfile

Matplotlib and numpy are required to run.


Compiling
---------

To compile the code:

	$ make -j2


Running
-------

To run the code:

	$ ./2DSOR -h
	Usage: ./2DSOR [option]...
	Options:
		-N	grid size in each dimension
		-t	max number of iterations
		-p	desired precision
		-g	desired precision
		-h	this text

Examples can be found in folder ../run/


Results
-------

The Python script plotter.py can be used to plot the output files:

	$ python3 plotter.py after.sol

With after.sol the output file.
