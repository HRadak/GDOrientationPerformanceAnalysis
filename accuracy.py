#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 26 14:16:54 2023

@author: mimetik
"""

import matplotlib.pyplot as plt
import csv
import numpy as np
import os
import glob
from scipy import signal

def getAccuracy():
    folder = "./SyntheticData/Results/"
    filelist = sorted( filter( os.path.isfile, glob.glob(folder + 'convergence/*') ) )
    filelist_euler = sorted( filter( os.path.isfile, glob.glob(folder + '*') ) )

    beta = []
    conv_times = np.zeros((54,9,len(filelist)))
    new_conv_times = np.zeros((54,9))
    
    data_all = np.zeros((54,9,len(filelist)))

    accuracy = np.zeros((54,9,len(filelist)))
    avg_accuracy = np.zeros((54,9))
    file_num = 0

    for filename in (filelist):
        if filename.endswith(".csv"): 
            print(filename)
            data = np.loadtxt(filename, delimiter="\t", dtype=np.float64)  # load csv
            new_conv_times[:,:] = data[:,0:9]    
            conv_times[:,:,file_num] = new_conv_times[:,:]

        file_num += 1

#    file_num = 0

#    for filename in (filelist_euler):
#        if filename.endswith(".csv"): 
#            print(filename)
#            data = np.loadtxt(filename, delimiter="\t", dtype=np.float64)  # load csv
#            data_all[:,:,file_num] = data[:,:]
            
    for i in range(conv_times.shape[0]):
        for j in range(conv_times.shape[1]):
            conv_time = conv_times[i,j, file_num]
            if conv_time != 100:
                accuracy[i,j,file_num] = np.average(data[conv_time*100:, j+3, file_num])
        file_num += 1
        
    
    beta[:] = data[:,-1]

    avg_accuracy = np.zeros((len(beta),9), dtype = np.float64)
    avg_accuracy[:,:] = np.average(accuracy[:,:,:], axis = 2)
    
    with open(folder + 'accuracy.csv', 'w') as f:
        for i in range(len(beta)):
            for j in range(avg_accuracy.shape[1]):
                f.write("%s\t" % avg_accuracy[i,j])
            f.write("%s\n" % beta[i])
#        f.write("\n")

if __name__ == '__main__':
    getAccuracy()