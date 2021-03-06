from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np

import argparse

def plotter(fname, outname):
        fig = plt.figure()
        ax = fig.gca(projection='3d')
        
        a = np.genfromtxt(fname, comments='  ', usecols=3)
        N = int(a)
        data = np.loadtxt(fname)
        
        x = np.arange(0, N)
        y = np.arange(0, N)
        
        X, Y = np.meshgrid(x, y)
        
        ax.plot_surface(X, Y, data, rstride=1, cstride=1, cmap=cm.ocean, linewidth=0, antialiased=True)

        ax.set_xlabel(r'$x$')
        ax.set_ylabel(r'$y$')
        ax.set_zlabel(r'$f(x,y)$')

        ax.set_axis_bgcolor('grey')

        if outname:
            fig.savefig(outname, dpi=fig.dpi)
        else:
            plt.show()

if __name__ == '__main__':
        parser = argparse.ArgumentParser()
        parser.add_argument("inp", help="file to plot")
        parser.add_argument("--outp", help="file to save plot")
        args = parser.parse_args()

        if args.outp:
            plotter(args.inp, args.outp)
        else:
            plotter(args.inp, outname='')
