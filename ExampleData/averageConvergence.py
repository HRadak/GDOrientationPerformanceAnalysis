#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Apr 23 18:40:35 2023

@author: hristinaradak95@gmail.com
"""

import matplotlib.pyplot as plt
import csv
import numpy as np
import os
import glob
from scipy import signal

folder = "./Results/convergence"

beta = []
new_times = np.zeros((54,9))
filelist = []

def getFilelist(folder, base_name):
#    filelist = sorted( filter( os.path.isfile, glob.glob(folder + '/' + base_name + '*') ) )
    filelist = sorted(glob.glob(folder + '/' + base_name + '*') )
    return filelist

def readData(filelist):
    file_num = 0
    times = np.zeros((54,9,len(filelist)))
    for filename in (filelist):
        if filename.endswith(".csv"):
#            print("Filename in readData", filename)
            new_data = np.loadtxt(filename, delimiter="\t", dtype=np.float64)  # load csv
            new_times[:,:] = new_data[:,0:9]
            times[:,:,file_num] = new_times[:,:]
        file_num += 1
    beta[:] = new_data[:,-1]
    return times, beta

    
def averageConvergence(times, beta):    
    avg_times = np.zeros((len(beta),9), dtype = np.float64)
    avg_times[:,:] = np.average(times[:,:,:], axis = 2)
#    avg_times[:,-1] = beta
    return avg_times

def averageConvergenceToTrue(times, beta):
    avg_times = averageConvergence(times, beta)
    for i in range(avg_times.shape[0]):
        for j in range(avg_times.shape[1]):
            if avg_times[i,j] > 99:
                avg_times[i,j] = -1
            else:
                avg_times[i,j] = 1
#    avg_times[:,-1] = beta
    return avg_times


def writeDataToFile(path, file_name, data):
#    file_extension = 'average_convergence.csv'
    with open(path + "/" + file_name, 'w') as f:
        for i in range(len(beta)):
            for j in range(data.shape[1]):
                f.write("%s\t" % data[i,j])
            f.write("%s\n" % beta[i])
    print("Data written to file", path + "/" + file_name)
    return 0

def plotHistogram(data):
        # An "interface" to matplotlib.axes.Axes.hist() method
    n, bins, patches = plt.hist(x=data, bins='auto', color='#0504aa',
                                alpha=0.7, rwidth=0.85)
    plt.grid(axis='y', alpha=0.75)
    plt.xlabel('Value')
    plt.ylabel('Frequency')
    plt.title('Histogram')
#    plt.text(23, 45, r'$\mu=15, b=3$')
    maxfreq = n.max()
    # Set a clean upper y-axis limit.
    plt.ylim(ymax=np.ceil(maxfreq / 10) * 10 if maxfreq % 10 else maxfreq + 10)
    plt.show()
    return 0

if __name__ == '__main__':
    folderlist = getFilelist(folder, '/convergence_')
    for folder in folderlist:    
        filelist = getFilelist(folder, '0')
        times, beta = readData(filelist)
        avg_times = averageConvergence(times, beta)
        writeDataToFile(folder, 'average_convergence.csv', avg_times)
    
    ### get avg convergence time from the stronger condition
    folderlist_true = getFilelist(folder, '/convergence_true_')
    for folder in folderlist_true:
        print("TO TRUE")
        filelist = getFilelist(folder, '0')
        times, beta = readData(filelist)
        avg_times_true = averageConvergenceToTrue(times, beta)
        writeDataToFile(folder, "average_convergence_true.csv", avg_times_true)
#    plotHistogram(avg_times[:,0])