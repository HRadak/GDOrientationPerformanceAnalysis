#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jan 20 10:59:00 2022

@author: hristina
"""
import numpy as np
import matplotlib.pyplot as plt
import math
from Quaternion import Quaternion


EulerAngles = np.zeros((3, 5))
relAngles = np.zeros((3, 5))
q_dot = np.zeros((4, 5), dtype = np.double)
q = np.zeros((4, 5), dtype = np.double)
q2 = np.zeros((4, 5), dtype = np.double)
gyro = np.zeros((4, 5))

EulerAngles[:,1] = [math.pi/2,math.pi/2,0]
EulerAngles[:,2] = [math.pi/2,math.pi/2,math.pi/2]
relAngles[:,1] = [math.pi/2,math.pi/2,0]
relAngles[:,2] = [0,0,math.pi/2]
q[0,0] = 1
q2[0,0] = 1

for i in range(0, 4):
    gyro[1:4,i] = relAngles[:,i]*100 #rad/s
    
for i in range(0, 4):
    q[:,i+1] = Quaternion.EulerAngles_to_Quaternion(EulerAngles[0,i+1],EulerAngles[1,i+1],EulerAngles[2,i+1])
    q[:,i+1] = q[:,i+1]/np.linalg.norm(q[:,i+1])

    q_dot[:,i+1] = Quaternion.EulerAngles_to_Quaternion(relAngles[0,i],relAngles[1,i],relAngles[2,i])
    q_dot[:,i+1] = 0.5*Quaternion.Quaternion_product(q2[:,i],gyro[:,i])
    q2[:,i+1] = q2[:,i] + q_dot[:,i]*0.01
    q[:,i+1] = q[:,i+1]/np.linalg.norm(q[:,i+1])
    q2[:,i+1] = q[:,i+1]/np.linalg.norm(q2[:,i+1])

## Calculate true quaternion from true gyro measurement
#for i in range(0, n_size-1):
#    q_dot[:,i+1] = 0.5*Quaternion.Quaternion_product(gyro[:,i], q[:,i])
#    q[:,i+1] = q[:,i] + q_dot[:,i+1]*0.01 #sampling time = 0.01s
#    q[:,i+1] = q[:,i+1]/np.linalg.norm(q[:,i+1])
#
## Calculate true quaternion from rel change in orientation (relative Euler angles)
#for i in range(0, n_size-1):
#    q_dot2[:,i] = Quaternion.EulerAngles_to_Quaternion(relEulerAngles[0,i+1],relEulerAngles[1,i+1],relEulerAngles[2,i+1])
#    q2[:,i+1] = Quaternion.Quaternion_product(q_dot2[:,i], q2[:,i])
#    q2[:,i+1] = q2[:,i+1]/np.linalg.norm(q[:,i+1])
diff = abs(np.subtract(q, q2))
plt.plot(diff[0,0:3], 'r', label = 'Difference qw')
plt.plot(diff[1,0:3], 'g', label = 'Difference qx')
plt.plot(diff[2,0:3], 'b', label = 'Difference qy')
plt.plot(diff[3,0:3], 'y', label = 'Difference qz')
plt.xlabel('Number of samples')
plt.legend()
#plt.savefig('/home/hristina/Documents/Hristina/sense-hristina/Results/20211216/yaw_rotation_smooth/diff_qx.eps', bbox_inches='tight')
plt.show()  