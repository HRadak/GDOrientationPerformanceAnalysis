#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep  3 15:47:01 2020

@author: hristinaradak95@gmail.com
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import itertools
import json
from matplotlib.backends.backend_pdf import PdfPages as pdfpages
import sys
import scipy
import os
from numpy.core.numeric import full
import random
import json
from enum import Enum
import csv
from array import array     
import matplotlib.patches as mpatches
from matplotlib.lines import Line2D
import glob

plt.style.use(['seaborn-pastel', 'presentation.mplstyle'])
plt.rcParams['axes.linewidth'] = 0.3
plt.rcParams['grid.linestyle'] = '--'
plt.rcParams['grid.linewidth'] = 0.2
plt.rcParams['axes.labelpad'] = 2 #padding between axis label and axis numbers
plt.rcParams['hatch.linewidth'] = 0.5
plt.rcParams['lines.linewidth'] = 0.5
plt.rcParams['hatch.color'] = 'w'
#plt.rcParams['text.usetex'] = True
#plt.rcParams['font.family'] = ['times new roman']
plt.rc('font', **{'family': 'serif', 'serif': ['Computer Modern']})
plt.rc('text', usetex=True)

print(plt.style.available)

GYR_COL = TRUE_COL = CONV_MADGWICK      = 0
ACC_COL = WILSON_COL = CONV_WILSON      = 3
MAG_COL = MADGWICK_COL = CONV_QGD       = 6
X_AXIS = ROLL = 0
Y_AXIS = PITCH = 1
Z_AXIS = YAW  = 2

markers = ['o', '^', 's', 'p', 'h']
marker_list = ['o', '^', 's', 'p', 'h', '*', 'D', 'v', '>']
line_styles = ['solid', 'dotted', 'dashed', 'dashdot']
g = itertools.cycle(marker_list)

basecolors = ['blue', 'green','cyan', 'red', 'magenta', 'SkyBlue', 'IndianRed', 'lightgreen', 'salmon']
colors = ['violet','lightpink','cyan','maroon', 'deepskyblue', 'magenta' , 'red', 'blueviolet', 'green',
          'deepskyblue', 'deeppink', 'lime', 'brown' ]

#color_selection = ["#fdd42e","#2edcfe","#f92ec6"]
color_selection = ["#1b9e77","#d95f02","#7570b3"]

colorM = color_selection[0]
colorW = color_selection[1]
colorQ = color_selection[2]

width = 4.000 # original was 3.500
height = 4.0

filelist = []

def getFilelist(folder, base_name):
#    filelist = sorted( filter( os.path.isfile, glob.glob(folder + '/' + base_name + '*') ) )
    filelist = sorted(glob.glob(folder + '/' + base_name + '*') )
    return filelist

def read_csv_file(filename):
    df = pd.read_csv(filename)
    print("Contents in csv file:", df)
    return df

def createManualLabels():
    # manual labels
    handles, labels = plt.gca().get_legend_handles_labels()
    lineM = Line2D([0], [0], label='MDW', color=colorM, linewidth = 0.7)
    lineW = Line2D([0], [0], label='Wilson', color=colorW, linestyle='dashdot', linewidth = 0.7)
    lineQ = Line2D([0], [0], label='QGD', color=colorQ, linestyle='--', linewidth = 0.7)

    patchM = mpatches.Patch(color=colorM, label='MDW true', linewidth= 0.0)   
    patchW = mpatches.Patch(color=colorW, label='Wilson true', linewidth= 0.0)   
    patchQ = mpatches.Patch(color=colorQ, label='QGD true', linewidth= 0.0)   
    handles = ([lineM, patchM, lineW, patchW, lineQ, patchQ])
    
    return handles

def plot_line_true_vs_raw(axis_type=X_AXIS):
    df_raw = read_csv_file('./data/imu_data_raw.csv')
    df_ideal = read_csv_file('./data/imu_data_ideal.csv')

    time_values = []
    for i in range(len(df_raw)):
        time_values.append(0.01 * i)

    print('len of times:', len(time_values))
    fig, axs = plt.subplots(3, sharex=True)
    ## set margin for figure
    bottom = .17
    top = .93
    fig.subplots_adjust(left=.18, bottom=.17, right=0.97, top=.93)

    for subfig in np.arange(0, 3):
        axs[subfig].plot(time_values, df_raw.iloc[:, GYR_COL + subfig*3 + axis_type],
                         label='Observed', color=basecolors[7])
        axs[subfig].plot(time_values, df_ideal.iloc[:, GYR_COL + subfig*3 + axis_type],
                         label='True', linestyle=line_styles[2], color=basecolors[0])
        axs[subfig].grid(True)
        ## set position of yaxis
        ## (0, 0) represents the bottom left corner, (0.5, 0.5) center, and (1, 1) top right corner of the plot
        axs[subfig].yaxis.set_label_coords(-.15, .5)

    axs[0].set_ylabel(r'$\omega_x [deg/s]$')
    axs[1].set_ylabel(r'$a_x [m/s^2]$')
    axs[2].set_ylabel(r'$m_x [\mu T]$')

    plt.xlabel(r'$Time [s]$')

    # plt.legend(loc='best')
    plt.grid(True)

    ## set the legend position
    box = axs[0].get_position()
    axs[0].set_position([box.x0, box.y0 - box.height * 0.03,
                     box.width * 1, box.height * 0.81])
    axs[0].legend(loc='upper center', bbox_to_anchor=(0., 1.4, 1., .100),
               fancybox=True, shadow=False, ncol=2, mode='expand', borderaxespad=0.)

    ## change the x-axis and y-axis ticks
    axs[0].tick_params(axis='both', width=0.3)
    axs[1].tick_params(axis='both', width=0.3)
    axs[2].tick_params(axis='both', width=0.3)
    
    axs[0].set_yticks([1, 0, -1])
    axs[1].set_yticks([10, 0, -10])
    axs[2].set_yticks([50, 0, -50])
    
    axs[0].set_ylim([-1.1, 1.1])
    axs[1].set_ylim([-12,12])
    axs[2].set_ylim([-80, 60])

    height = 2.5
    fig.set_size_inches(width, height)
    fig.savefig('../figures/fig3_' + str(axis_type) + '.pdf')
    plt.close()

def plot_line_madg_will_true(axis_type=0):
    df = read_csv_file('./data/ahrs.csv')
    time_values = []

    for i in range(len(df)):
        time_values.append(0.01 * i)

    print('len of times:', len(time_values))
    fig, axs = plt.subplots(3, sharex=True)
    ## set margin for figure
    fig.subplots_adjust(left=.18, bottom=.17, right=0.97, top=.93)

    for subfig in np.arange(0, 3):
        axs[subfig].plot(time_values, df.iloc[:, TRUE_COL + subfig],
                         label='True', color=basecolors[6],  linestyle='--')
        axs[subfig].plot(time_values, df.iloc[:, MADGWICK_COL + subfig],
                         label='Madgwick', color=basecolors[0], linestyle='dashdot')
        axs[subfig].plot(time_values, df.iloc[:, WILSON_COL + subfig],
                         label='Wilson',  color=basecolors[2])
        axs[subfig].grid(True)
        ylabel = 'Roll'
        if subfig == 1: ylabel = 'Pitch'
        elif subfig == 2: ylabel = 'Yaw'
        axs[subfig].set_ylabel((r'${}~[deg]$').format(ylabel))
        ## set position of yaxis
        ## (0, 0) represents the bottom left corner, (0.5, 0.5) center, and (1, 1) top right corner of the plot
        axs[subfig].yaxis.set_label_coords(-.15, .5)

    plt.xlabel(r'$Time [s]$')

    # plt.legend(loc='best')
    plt.grid(True)

    ## set the legend position
    box = axs[0].get_position()
    axs[0].set_position([box.x0, box.y0 - box.height * 0.03,
                         box.width * 1, box.height * 0.81])
    axs[0].legend(loc='upper center', bbox_to_anchor=(0., 1.4, 1., .100),
                  fancybox=True, shadow=False, ncol=3, mode='expand', borderaxespad=0.)

    ## change the x-axis and y-axis ticks
    axs[0].tick_params(axis='both', width=0.3)
    axs[1].tick_params(axis='both', width=0.3)
    axs[2].tick_params(axis='both', width=0.3)

    ## limit y-axis values
#    axs[0].set_ylim(np.min(df.iloc[:, WILSON_COL + ROLL]), np.max(df.iloc[:, MADGWICK_COL + ROLL]))
    axs[0].set_yticks([100, 0, -100])
    axs[1].set_yticks([50, 0, -50])
    axs[2].set_yticks([50, 0, -50])
    
    axs[0].set_ylim([-120, 100])
    axs[1].set_ylim([-80,60])
    axs[2].set_ylim([-50, 90])

    height = 2.5
    fig.set_size_inches(width, height)
    fig.savefig('../figures/ahrs.pdf')

    plt.close()

def plot_convergence_madg_will_true(axis_type=0):
    df = read_csv_file('./data/convergence1_3_10.csv')
    time_values = []

    for i in range(len(df)):
        time_values.append(0.01 * i)

    print('len of times:', len(time_values))
    fig, axs = plt.subplots(3, sharex=True)
    ## set margin for figure
    fig.subplots_adjust(left=.18, bottom=.17, right=0.97, top=.93)

    for subfig in np.arange(0, 3):
        axs[subfig].plot(time_values, df.iloc[:, TRUE_COL + subfig],
                         label='True', color=basecolors[6],  linestyle='--')
        axs[subfig].plot(time_values, df.iloc[:, MADGWICK_COL + subfig],
                         label='Madgwick', color=basecolors[0], linestyle='dashdot')
        axs[subfig].plot(time_values, df.iloc[:, WILSON_COL + subfig],
                         label='Wilson',  color=basecolors[2])
        axs[subfig].grid(True)
        ylabel = 'Yaw'
#        if subfig == 1: ylabel = 'Pitch'
#        elif subfig == 2: ylabel = 'Yaw'
        axs[subfig].set_ylabel((r'${}~[deg]$').format(ylabel))
        ## set position of yaxis
        ## (0, 0) represents the bottom left corner, (0.5, 0.5) center, and (1, 1) top right corner of the plot
        axs[subfig].yaxis.set_label_coords(-.15, .5)

    plt.xlabel(r'$Time [s]$')

    # plt.legend(loc='best')
    plt.grid(True)

    ## set the legend position
    box = axs[0].get_position()
    axs[0].set_position([box.x0, box.y0 - box.height * 0.03,
                         box.width * 1, box.height * 0.81])
    axs[0].legend(loc='upper center', bbox_to_anchor=(0., 1.4, 1., .100),
                  fancybox=True, shadow=False, ncol=3, mode='expand', borderaxespad=0.)

    ## change the x-axis and y-axis ticks
    axs[0].tick_params(axis='both', width=0.3)
    axs[1].tick_params(axis='both', width=0.3)
    axs[2].tick_params(axis='both', width=0.3)

    ## limit y-axis values
#    axs[0].set_ylim(np.min(df.iloc[:, WILSON_COL + ROLL]), np.max(df.iloc[:, MADGWICK_COL + ROLL]))
    for i in range(3):
        axs[i].set_ylim([-200,15])
        axs[i].set_yticks([0, -100, -200])
        
    # place a text box in upper left in axes coords
    axs[0].text(8, -100, r'$\beta = 1$', verticalalignment='center')
    axs[1].text(8, -100, r'$\beta = 3$', verticalalignment='center')
    axs[2].text(8, -100, r'$\beta = 10$')

    height = 2.5
    fig.set_size_inches(width, height)
    fig.savefig('../figures/fig5_' + str(axis_type) + '.pdf')

    plt.close()
    
def plot_convergence_time(folder):
#    df = open('./SyntheticData/Results/convergence/average_convergence.csv', 'r')
    df = open(folder + '/average_convergence.csv', 'r')

    data = np.loadtxt(df, delimiter="\t", dtype=np.float64)
    betas = data[:,-1]
    
    print('len of times:', len(betas), betas)
    fig, axs = plt.subplots(3, sharex=True)
    ## set margin for figure
    fig.subplots_adjust(left=.18, bottom=.17, right=0.97, top=.87)

    for subfig in np.arange(0, 3):
        axs[subfig].plot(betas, data[:, CONV_MADGWICK + subfig],
                         label='MDW', color=colorM, linewidth = 0.7)
        axs[subfig].plot(betas, data[:, CONV_WILSON + subfig],
                         label='Wilson', color=colorW, linestyle='dashdot', linewidth = 0.7)
        axs[subfig].plot(betas, data[:, CONV_QGD + subfig],
                         label='QGD',  color=colorQ,  linestyle='--', linewidth = 0.7)
        axs[subfig].grid(True)
        ylabel = r'$T_{\rm s}[s]$'
        if subfig == 1: ylabel = r'$T_{\rm s}[s]$'
        elif subfig == 2: ylabel = r'$T_{\rm s}[s]$'
        axs[subfig].set_ylabel(ylabel)
#        axs[subfig].set_ylabel((r'${}~[s]$').format(ylabel))
        ## set position of yaxis
        ## (0, 0) represents the bottom left corner, (0.5, 0.5) center, and (1, 1) top right corner of the plot
        axs[subfig].yaxis.set_label_coords(-.15, .5)

    plt.xlabel(r'$\beta $')

    # plt.legend(loc='best')
    plt.grid(True)

    ## set the legend position
    box = axs[0].get_position()
#    axs[0].set_position([box.x0, box.y0 - box.height * 0.03,
#                         box.width * 1, box.height * 0.81])
    axs[0].legend(loc='upper center', bbox_to_anchor=(0., 1.4, 1., .100),
                  fancybox=True, shadow=False, ncol=3, mode='expand', borderaxespad=0.)

    ## change the x-axis and y-axis ticks
    axs[0].tick_params(axis='both', width=0.3)
    axs[1].tick_params(axis='both', width=0.3)
    axs[2].tick_params(axis='both', width=0.3)

    ## limit y-axis values
#    axs[0].set_ylim(np.min(df.iloc[:, WILSON_COL + ROLL]), np.max(df.iloc[:, MADGWICK_COL + ROLL]))
    for i in range(3):
        axs[i].set_ylim([-5,105])
        axs[i].set_yticks([0, 50, 100])
        axs[i].set_xscale('log')

        
    # place a text box in upper left in axes coords
    axs[0].text(1800, 20, 'roll', verticalalignment='center', color='black', fontsize = 9)
    axs[1].text(1600, 20, 'pitch', verticalalignment='center', color='black', fontsize = 9)
    axs[2].text(1800, 16, 'yaw', color='black', fontsize = 9)

    height = 3.5
    fig.set_size_inches(width, height)
    fig.savefig(folder + '/average_convergence' + '.pdf')

    plt.show()
    plt.close()
    
def plot_convergence_to_true_time(folder):
    file_num = folder[-1]
    print("file num " , file_num)
    df = open(folder + '/average_convergence.csv', 'r')
    folder_true = folder[0:-14] + '/convergence_true_' + file_num
    print("folder_true " , folder_true)

    df_true = open(folder_true+ '/average_convergence_true.csv', 'r')

    data = np.loadtxt(df, delimiter="\t", dtype=np.float64)
    betas = data[:,-1]
    
    data_true = np.loadtxt(df_true, delimiter="\t", dtype=np.float64)

    pos = 2
    linewidth = np.zeros((data_true.shape[0], data_true.shape[1]))

    ## convert to 0 or 1 and move to negative part of y axis to plot it below the actual graph
    for algo in range(3):
        for axis in range(3):
            for i in range(data_true.shape[0]):
                if data_true[i,axis+algo*3] == 1:
                    linewidth[i,axis+algo*3] = pos
                else:
                    linewidth[i,axis+algo*3] = pos/3
                #align to the same position
                data_true[i,axis+algo*3] = - pos*3*(algo+1)-2
    
                
#    print('len of times:', len(betas), betas)
    fig, axs = plt.subplots(3, sharex=True)
    ## set margin for figure
    fig.subplots_adjust(left=.18, bottom=.1, right=0.97, top=.87)

    for subfig in np.arange(0, 3):
        axs[subfig].plot(betas, data[:, CONV_MADGWICK + subfig],
                         label='MDW', color=colorM, linewidth = 0.7)
        axs[subfig].plot(betas, data[:, CONV_WILSON + subfig],
                         label='Wilson', color=colorW, linestyle='dashdot', linewidth = 0.7)
        axs[subfig].plot(betas, data[:, CONV_QGD + subfig],
                         label='QGD',  color=colorQ,  linestyle='--', linewidth = 0.7)
        axs[subfig].grid(True)
        ylabel = r'$T_{\rm w}[s]$'
        if subfig == 1: ylabel = '$T_{\rm w}[s]$'
        elif subfig == 2: ylabel = '$T_{\rm w}[s]$'
        axs[subfig].set_ylabel(ylabel)
#        axs[subfig].set_ylabel((r'${}~[s]$').format(ylabel))
        ## set position of yaxis
        ## (0, 0) represents the bottom left corner, (0.5, 0.5) center, and (1, 1) top right corner of the plot
        axs[subfig].yaxis.set_label_coords(-.15, .5)
        
        for i in range(data_true.shape[0]):
            axs[subfig].plot(betas[i:i+2], data_true[i:i+2,subfig], color=colorM, linestyle='-', linewidth = linewidth[i,subfig])
            axs[subfig].plot(betas[i:i+2], data_true[i:i+2,subfig+3], color=colorW, linestyle='-', linewidth = linewidth[i,subfig+3])
            axs[subfig].plot(betas[i:i+2], data_true[i:i+2,subfig+6], color=colorQ, linestyle='-', linewidth = linewidth[i,subfig+6])

    plt.xlabel(r'$\beta $')
    plt.grid(True)
    
    handles = createManualLabels()
    
    ## set the legend position
    box = axs[0].get_position()
#    axs[0].set_position([box.x0, box.y0 - box.height * 0.3,
#                         box.width * 1, box.height * 1])
    axs[0].legend(handles = handles, loc='upper left', bbox_to_anchor=(0., 1.4, 1., .100),
                  fancybox=True, shadow=False, ncol=3, mode='expand', borderaxespad=0)

    ## change the x-axis and y-axis ticks
    axs[0].tick_params(axis='both', width=0.3)
    axs[1].tick_params(axis='both', width=0.3)
    axs[2].tick_params(axis='both', width=0.3)

    ## limit y-axis values
#    axs[0].set_ylim(np.min(df.iloc[:, WILSON_COL + ROLL]), np.max(df.iloc[:, MADGWICK_COL + ROLL]))
    for i in range(3):
        axs[i].set_ylim([-30,105])
        axs[i].set_yticks([0, 50, 100])
        axs[i].set_xscale('log')

    # place a text box in upper left in axes coords
    axs[0].text(1800, 20, 'roll', verticalalignment='center', color='black', fontsize = 9)
    axs[1].text(1600, 20, 'pitch', verticalalignment='center', color='black', fontsize = 9)
    axs[2].text(1800, 16, 'yaw', color='black', fontsize = 9)
    
    height = 3.5
    fig.set_size_inches(width, height)
    fig.savefig(folder + '/convergence_to_true' + '.pdf')
    
    plt.show()
    plt.close()

if __name__ == '__main__':
    # plot fig. 3
#    for axis in [X_AXIS, Y_AXIS, Z_AXIS]:
#        plot_line_true_vs_raw(axis_type=axis)

    ## plot fig. 4
#    plot_line_madg_will_true(axis_type=ROLL)
    # plot fig. 5
#    plot_convergence_madg_will_true(axis_type=ROLL)
    ## plot fig. convergence time logarithmic
    
    ## plot fig. convergence weak
    folder = "./Results/convergence"
    folderlist = getFilelist(folder, '/convergence_')
    for folder in folderlist:  
        plot_convergence_time(folder)
    
    ## plot fig. with True/False values below
#    for folder in folderlist[0:5]:
    for folder in folderlist[0:1]:
        plot_convergence_to_true_time(folder)
