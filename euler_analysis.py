#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep  3 15:47:01 2020

@author: hristina
"""
import matplotlib.pyplot as plt
import csv
import numpy as np
import os
import glob
from scipy import signal
import sys
from scipy.signal import lfilter, lfilter_zi
import shutil

#diff_max = [0.125, 0.25, 0.5, 1, 2]
diff_max = [0.125]

folderOut = "./SyntheticData/Results_m1_w_qgd/convergence/"

def deleteFiles(folder):
#    filelist = glob.glob(folder + "*")
    filelist = sorted(glob.glob(folder + '*') )
    for f in filelist:
        shutil.rmtree(f)

def makeFolders(folder, base_name, array1d):
    folders = []
    for i in range(len(array1d)):
        path = os.path.join(folderOut, base_name + str(i))
        if not os.path.exists(path):
            os.mkdir(path)
        folders.append(path)
    return folders

def writeToFile(folder, iteration, data, param1, param2):
    with open(folder + '/' + iteration + '.csv', 'a+') as f:
        for i in range(len(data)):
            f.write("%s\t" % data[i])
        f.write("%s\t" % param1)
        f.write("%s\n" % param2)
    return 0
    
def testConvergence(folderIn):
    time_axis = []
    # the filter size has to be a even number in order to get an odd number of samples to filter
    filter_size = 10
    
    iteration = str(folderIn[-4:])
    
    filelist = sorted( filter( os.path.isfile, glob.glob(folderIn + '/*.csv') ) )
#    filelist = [folderIn + "/0000.csv", folderIn + "/0001.csv", folderIn + "/0002.csv", folderIn + "/0003.csv", folderIn + "/0004.csv"]
#    filelist = [folderIn + "/0018.csv"]

    i=0
    j=0
    
    a = np.ones(1)
    b = np.ones(filter_size+1)
    b /= filter_size + 1
    zi = lfilter_zi(b, a)

    num_file = 0
    convergence = np.ones((len(filelist),9), dtype=np.double)
    convergence_true = np.ones((len(filelist),9), dtype=np.double)
    convergence *= 100
    convergence_true *= 100
    
    for filename_ea in (filelist):
        print ("filename", filename_ea)
        xT = []
        yT = []
        zT = []
        #mdw
        xM = []
        yM = []
        zM = []
        #wilson
        xW = []
        yW = []
        zW = []
        #QGD
        xQ = []
        yQ = []
        zQ = []
            
        beta = []
        
        with open(filename_ea, 'r') as csvfile:
            plots = csv.reader(csvfile, delimiter=',')
            for row in plots:
                xT.append(float(row[0])) 
                yT.append(float(row[1]))
                zT.append(float(row[2]))
                
                xM.append(float(row[3]))
                yM.append(float(row[4]))
                zM.append(float(row[5]))
                
                xW.append(float(row[6]))
                yW.append(float(row[7])) # - for MDW2 
                zW.append(float(row[8])) # - for MDW2
                
                xQ.append(float(row[9]))
                yQ.append(float(row[10]))
                zQ.append(float(row[11])) 
                
                beta.append(float(row[12]))
    
        n_size = len(xM)
        data = np.zeros((n_size,12), dtype=np.float64)
        diff = np.zeros((n_size,9), dtype=np.float64)
#        diffT = np.zeros((n_size,9), dtype=np.float64)
        diff_filtered = np.zeros((n_size,9), dtype=np.float64)
        
        # convert roll for MDW2
        for i in range(n_size):
            if xW[i] > 0:
                xW[i] -= 180
            else:
                xW[i] += 180

        data[:,0] = xM
        data[:,1] = yM
        data[:,2] = zM
        data[:,3] = xW
        data[:,4] = yW
        data[:,5] = zW
        data[:,6] = xQ
        data[:,7] = yQ
        data[:,8] = zQ
        data[:,9] = xT
        data[:,10] = yT
        data[:,11] = zT
        
        time_axis.clear()
        
        for l in range(n_size):
            time_axis.append(0.01*l)
    
        #############################################################################################
        ########## Difference from the true value of the respective algorithms ######################
        #############################################################################################
    
#        diffT[:,0] = (np.subtract(xT, xM))
#        diffT[:,3] = (np.subtract(xT, xW))
#        diffT[:,6] = (np.subtract(xT, xQ))
#          
#        diffT[:,1] = abs(np.subtract(yT, yM))
#        diffT[:,4] = abs(np.subtract(yT, yW))
#        diffT[:,7] = abs(np.subtract(yT, yQ))
#       
#        diffT[:,2] = abs(np.subtract(zT, zM))
#        diffT[:,5] = abs(np.subtract(zT, zW))
#        diffT[:,8] = abs(np.subtract(zT, zQ))
        
        #############################################################################################
        ########## Difference from the consecutive samples ######################
        #############################################################################################

        for j in range(9):
            for i in range(len(xT)):
                diff[i,j] = data[i,j] - data[i,j%3+9]   
        diff *= diff
               
        for j in range(diff.shape[1]):
            diff_filtered[:,j], zf = signal.lfilter(b, a, diff[:,j], axis=0, zi=zi*diff[0,j]) # com / uncom
            # calculate RMSD in a window of size filter_size
            diff_filtered[:,j] /= filter_size
            diff_filtered[:,j] = np.sqrt(diff_filtered[:,j])          
            
        for k in range(len(diff_max)):
            for j in range(diff.shape[1]):
                for i in reversed(range(diff.shape[0])):
                    if(abs(diff[-1,j] - diff[i,j]) > diff_max[k]):
                        convergence[num_file,j] = round(float(i)*0.01,1)
                        break
       
            for j in range(diff.shape[1]):
                for i in reversed(range(diff.shape[0])):
                    if(diff_filtered[i,j] > diff_max[k]):
                        convergence_true[num_file,j] = round(float(i)*0.01,1)
                        break

            writeToFile(convergence_folders[k], iteration, convergence[num_file,:], diff_max[k], beta[0])
            writeToFile(convergence_true_folder[k], iteration, convergence_true[num_file,:], diff_max[k], beta[0])

        num_file += 1

if __name__ == '__main__':
    folderIn = sys.argv[1]
#    folderIn = "./SyntheticData/Results/eulerResult_0333"
    convergence_folders = makeFolders(folderOut, "convergence_", diff_max)
    convergence_true_folder = makeFolders(folderOut, "convergence_true_", diff_max)
    
    iteration = str(folderIn[-4:])
    print("Iteration", iteration)
    testConvergence(folderIn)