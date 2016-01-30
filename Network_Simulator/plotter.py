import matplotlib as mpl
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cbook as cbook

import sys

#
# @author:metastableB
# plotter.py
# 
'''
argv[2] : x label 
argv[3] : y label 
argv[4] : title and legends
argv[5] : image name
'''

x = []
y = []

with open(sys.argv[1]) as fp:
    for line in fp:
		t = [x_.strip() for x_ in line.split('\t')]
		x.append(t[0])
		y.append(t[1])

fig = plt.plot(x,y,'ro')

plt.xlabel(sys.argv[2].replace("_"," "))
plt.ylabel(sys.argv[3].replace("_"," "))
plt.grid(True)
plt.title(sys.argv[4].replace("_"," "))
plt.savefig(sys.argv[5])
plt.show();

