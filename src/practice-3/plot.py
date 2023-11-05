#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

def draw(filenames, labels, dest_filename):
    plt.rcParams["legend.markerscale"] = 1.0
    plt.rcParams['font.family'] = 'sans-serif'
    plt.rcParams['font.sans-serif'] = ['PT Sans']
    plt.rcParams['font.size'] = '9'
    plt.rcParams["legend.loc"] = "upper left"
    plt.rcParams["legend.fontsize"] = '7'
    cm = 1 / 2.54 # centimeters in inches
    fig = plt.figure(figsize = (10 * cm, 7 * cm))
    ax = fig.add_subplot(111)
    ax.set_title("")
    ax.set(xlabel = "Number of processes", ylabel = "Relative speedup")
    ax.label_outer()
    #ax.xaxis.set_ticks(np.arange(4, 16, 1))
    #ax.yaxis.set_ticks(np.arange(1.8, 9, 1))
    ax.xaxis.set_tick_params(direction='in', which='both')
    ax.yaxis.set_tick_params(direction='in', which='both')
    #ax.grid()
    for (fname, datalabel) in zip(filenames, labels):
        data = np.loadtxt(fname)
        x = data[:, 0]
        y = data[:, 1]
        if datalabel == "N = 28000":
            marker = '-^'
            color = "dodgerblue"
        elif datalabel == "N = 45000":
            marker = '-s'
            color = "orange"
        else:
            marker = '-'
            color = "red"
        ax.plot(x, y, marker, c = color, markersize = 4.0, linewidth = 1.2, label = datalabel)
    plt.tight_layout()
    ax.legend()
    fig.savefig(dest_filename, dpi = 300)

if __name__ == "__main__":
    draw(["sgemv_28.dat", "sgemv_45.dat", "linear.dat"], ["N = 28000", "N = 45000", "Linear speedup"], "sgemv.png")
