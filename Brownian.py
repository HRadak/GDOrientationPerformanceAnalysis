#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  8 16:31:20 2021

@author: Hristina
"""

import numpy as np

class Brownian():
    """
    A Brownian motion class constructor
    """
    def __init__(self,x0=0):
        """
        Init class
        """
        assert (type(x0)==float or type(x0)==int or x0 is None), "Expect a float or None for the initial value"
        
        self.x0 = float(x0)
    
    def gen_data(self,n_step=100, var=1, abs_=False):
        """
        Generate motion by random walk
        
        Arguments:
            n_step: Number of steps
            
        Returns:
            A NumPy array with `n_steps` points
        """
        # Warning about the small number of steps
        if n_step < 30:
            print("WARNING! The number of steps is small. It may not generate a good stochastic process sequence!")
        
        w = np.ones(n_step)*self.x0
        
        for i in range(1,n_step):
            # Sampling from the Normal distribution with probability 1/2
            if abs_==False :
                yi = np.random.normal(0,var) #change variance here
            else:
                yi = abs(np.random.normal(0,var)) #change variance here
            # Weiner process
            w[i] = w[i-1]+(yi/np.sqrt(n_step))
        
        return w
    """
    Generate additive white Gaussian noise
    """
    def gen_awgn(self, n_step=100, var=1):
        w = np.ones(n_step)*self.x0
        for i in range(1,n_step):
            w[i] = np.random.normal(0,var)
        return w
    """
    Generate motion with constant step size
    in only one direction
    """
    def gen_data2(self, n_step=100):
        w = np.zeros(n_step)*self.x0
        for i in range(1,n_step):
            #take step in one direction
            w[i] = w[i-1]+0.005   
        return w
    
    def gen_steady_data(self, n_step=100):
        w = np.zeros(n_step)*self.x0
        x = np.random.normal(0,0.9);            
        for i in range(1,n_step):
            w[i] = x
        return w
       
   
