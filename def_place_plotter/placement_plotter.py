'''
    File name      : placement_plotter.py
    Author         : Jinwook Jung
    Created on     : Mon 12 Aug 2019 11:54:11 PM EDT
    Last modified  : 2019-08-12 23:54:11
    Description    :
'''

import sys, json, gzip, logging
import matplotlib.pyplot as plt
from numpy import linspace
from matplotlib import cm
import logging

logger = logging.getLogger(__name__)
ch = logging.StreamHandler()
formatter = logging.Formatter('%(levelname)s - %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)


class Cell(object):
    def __init__(self, usage, def_name, x, y, w, h):
        self.usage = usage
        self.def_name = def_name

        self.x, self.y = x, y
        self.w, self.h = w, h

    def __str__(self):
        return "{} = {} {} {} {}".format(self.usage, self.x, self.y, self.w, self.h)

class Circuit(object):
    def __init__(self, name='Circuit'):
        self.name = name
        self.combs = list()
        self.latches = list()
        self.terminals = list()

    def read_boxes(self, file_name):
        with open(file_name, 'r') as f:
            lines = [l for l in (_.strip() for _ in f) if l]

        for line in lines:
            tokens = line.split()
            if tokens[0] == "#":
                continue

            def_name, usage = tokens[0:2]
            x, y, w, h = [float(t) for t in tokens[2:]]

            cell = Cell(usage, def_name, x, y, w, h)

            if def_name == "terminal":
                self.terminals.append(cell)
            elif usage.startswith("l"):
                self.latches.append(cell)
            else:
                self.combs.append(cell)

        print("Number of cells: %d" % (len(self.combs) + len(self.latches)))


def generate_plot(circuit):
    x_coords = [c.x for c in circuit.terminals]
    y_coords = [c.y for c in circuit.terminals]
    x_min, y_min = min(x_coords), min(y_coords)
    x_max, y_max = max(x_coords), max(y_coords)

    #---------------------------------------
    # Make placement plot
    #---------------------------------------
    from matplotlib.pyplot import Rectangle
    from matplotlib.collections import PatchCollection

    fig = plt.figure()
    fig.set_size_inches(10,10)
    ax = fig.add_subplot(1,1,1)

    # Plot the placement
    patches = set()

    for comb in circuit.combs:
        patches.add(Rectangle((comb.x, comb.y), comb.w, comb.h))
    ax.add_collection(PatchCollection(patches, facecolor='#147DFF', edgecolor='dimgrey', lw=0.25))

    patches = set()
    for latch in circuit.latches:
        patches.add(Rectangle((latch.x, latch.y), latch.w, latch.h))
    ax.add_collection(PatchCollection(patches, facecolor='#969696', edgecolor='dimgrey', lw=0.25))

#    ax.set_xlim([0, x_max])
#    ax.set_ylim([0, y_max])
    ax.autoscale()
    fig.savefig('foo2.png', dpi=200)


if __name__ == '__main__':
    """ Parse command line arguments. """
    import argparse
    parser = argparse.ArgumentParser()

    # Add arguments
    parser.add_argument('--placement', action='store', dest='src', required=True)
    opt = parser.parse_args()

    circuit = Circuit()
    circuit.read_boxes(opt.src)

    generate_plot(circuit)

