############################################################
# file: plot.py
# authors: Maxence Montavon & Damian Boquete Costa
# description: reading text file and showing graphics
############################################################

import sys
import tkinter
import matplotlib
from matplotlib import pyplot as plt

matplotlib.use('TkAgg')


# Reads a .txt file
def read_file(filename):
    f = open(filename, "r").read().split(',')

    x_tab = []
    y_tab = []
    for i in range(0, len(f)-1, 2):
        x_tab.append(float(f[i]))
        i+=1
        y_tab.append(float(f[i]))

    return x_tab, y_tab


# Build the graphics for groups
def build_figure(title, nrows, ncols, index, filename):
    x, y = read_file(filename)
    gradient = [x[-1], y[-1]]

    x = x[:len(x)-1]
    y = y[:len(y)-1]
    
    plt.subplot(nrows, ncols, index)
    plt.plot(x, y, 'b.')
    g_y = [gradient[0] * i + gradient[1] for i in x]
    plt.plot(x, g_y, 'r')
    plt.title(title)
    plt.legend(["Cloud of dots N=" + str(len(x)), "Gradient"], loc="upper left")
    plt.grid()


if __name__ == "__main__":
    x, y = read_file(sys.argv[1])
    gradient = [x[-1], y[-1]]

    x = x[:len(x)-1]
    y = y[:len(y)-1]

    plt.figure(1)
    plt.plot(x, y, 'b.')
    g_y = [gradient[0] * i + gradient[1] for i in x]
    plt.plot(x, g_y, 'r')
    plt.title('Gradient')
    plt.legend(["Cloud of dots N=" + str(len(x)), "Gradient"], loc="upper left")
    plt.grid()

    if len(sys.argv) == 5:
        plt.figure(2)
        build_figure("G1UG2 -> G3", 5,1,1, sys.argv[2])
        build_figure("G1UG3 -> G2", 5,1,3, sys.argv[3])
        build_figure("G2UG3 -> G1", 5,1,5, sys.argv[4])
    
    plt.show()
