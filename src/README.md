Source code	{#SourceCode}
===========


# Structure of Code	{#SourceCodeStructureOfCode}


## main		{#SourceCodemain}

The file main.c contains the command line interface to run the code as well as
the RHS function for the CPU code.


## PoissonSOR2D		{#SourceCodePoissonSOR2D}

Implementation of the algorithm is in PoissonSOR2D.c. Header file
PoissonSOR2D.h should be included to run the code.

It can be compiled with OpenMP support. See @ref SourceCodeCompiling


## PoissonSOR2D_CUDA	{#SourceCodePoissonSOR2DCUDA}

CUDA implementation of the algorithm is in PoissonSOR2D_CUDA.c. Header file
PoissonSOR2D_CUDA.h should be included to run the code.


## plotter	{#SourceCodeplotter}

PoissonSOR2D.h contains a function to write data do a file. The Python
script plotter.py plots this output file:

	$ python3 plotter.py outfile

Matplotlib and numpy are required to run.


# Compiling the code	{#SourceCodeCompiling} 

To compile the code without OpenMP support:

	$ cd src/
	$ make -j3

To enable OpenMP support, src/ directory myst be clean:

	$ cd src/
	$ make clean
	$ make OMP=1 -j3


# Running the code		{#SourceCodeRunning}

To run the code:

	$ ./2DSOR -h
	Usage: ./2DSOR [option]...
	Options:
		-N	grid size in each dimension
		-t	max number of iterations
		-p	desired precision
		-g	desired SOR parameter 
		-h	this text

Default values are:

	N = 128
	t = 4200
	p = 0.000001
	g ~ 1.95

Examples can be found in run/ folder. See @ref RunExamples for details.

## Output of the code	{#SourceCodeOutput}

The output of the code is the parameters used:

- grid size
- maximum number of iterations
- desired precision
- SOR parameter

After this parameters, the code will output at every 100 iterations the
iteration number, current norm and desired precision for the CPU version of the
code. GPU version of the code outputs first the grid and block sizes used, than
every 100 iteratins, the iteration number, current norm and desired precision.
After the runs, it is shown the time (in seconds) taken to run the CPU code and
GPU code (this one includes the memory transfers, GPU allocation and GPU free).

Two files will be created:

- cpu.sol
- gpu.sol


# Results	{#SourceCodeResults}

The Python script plotter.py can be used to plot the output files:

	$ python3 plotter.py -h
	usage: plotter.py [-h] [--outp OUTP] inp
	
	positional arguments:
	  inp          file to plot
	
	optional arguments:
	  -h, --help   show this help message and exit
	  --outp OUTP  file to save plot

Example:

	$ python3 plotter.py after.sol --outp fig.png

With after.sol the output file.
