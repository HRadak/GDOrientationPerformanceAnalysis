
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  8 16:37:11 2021

@author: Hristina
"""

import matplotlib.pyplot as plt
import numpy as np
import math
from Brownian import Brownian
from Quaternion import Quaternion
from GetAnglesFromQuaternion import *
from scipy.spatial.transform import Rotation as R


n_size = 5000 #how much data in total
sampling_time = 0.01 #in seconds
rel_euler = np.zeros((3,n_size))
rel_quat = np.zeros((4,n_size))
q = np.zeros((4,n_size))
EulerAngles = np.zeros((3, n_size))
euler_in_deg = np.zeros((3, n_size))

gyroT = np.zeros((4, n_size), dtype = np.double)
q_dot = np.zeros((4, n_size), dtype = np.double)


time_axis = []
q[:,0] = [1,0,0,0]

b = Brownian()
for i in range(n_size):
    time_axis.append(0.01*i)

for i in range(3):
    EulerAngles[i,:] = np.concatenate([np.sin(np.linspace(0, np.pi, 1500)), np.linspace(0,0,2000),np.sin(np.linspace(-np.pi, 3*np.pi, 1500))])

# Plot absolute Euler angles
plt.plot(time_axis, EulerAngles[0,:], 'r', label = "Roll")
plt.plot(time_axis, EulerAngles[1,:], 'g', label = "Pitch")
plt.plot(time_axis, EulerAngles[2,:], 'b', label = "Yaw")
plt.legend()
plt.xlabel('Time [s]')
plt.ylabel('Angle in [$deg$]')
plt.title("Absolute Euler Angles")
plt.savefig('/home/hristina/Desktop/abs_euler.eps', bbox_inches='tight')
plt.show()

#Calc. relative Euler Angles
for i in range(n_size-1):
    rel_euler[:,i] = EulerAngles[:,i+1] - EulerAngles[:,i]
    gyroT[1:4,i] = rel_euler[:,i]/sampling_time

# Plot relative Euler angles
plt.plot(rel_euler[0,:], 'r', label = "Roll")
plt.plot(rel_euler[1,:], 'g', label = "Pitch")
plt.plot(rel_euler[2,:], 'b', label = "Yaw")
plt.legend()
plt.xlabel('Number of samples')
plt.ylabel('Angle in [$rad$]')
plt.title("Relative Euler Angles")
#plt.savefig('/home/hristina/Documents/Hristina/phd/Research Topics/Orientation Estimation/Quaternion Gradient Descent/figures/abs_euler.eps', bbox_inches='tight')
plt.show()

for i in range(n_size-1):
    #Generate absolute Euler Angles
    r = R.from_euler('zxy', rel_euler[:,i], degrees=False)
    rel_quat[:,i] = r.as_quat()

# Calculate absolute quaternion
for i in range(n_size-1):
    r = R.from_euler('zxy', EulerAngles[:,i], degrees=False)
    q[:,i] = r.as_quat()
    q[:,i] = q[:,i]/np.linalg.norm(q[:,i])
    EulerAngles[:,i] = GetAnglesFromQuaternion(q[:,i])
    
# Plot true quaternion
plt.title("True Quaternion")
plt.plot(time_axis, q[0,:], 'r', label = "qw")
plt.plot(time_axis, q[1,:], 'g', label = "qx")
plt.plot(time_axis, q[2,:], 'b', label = "qy")
plt.plot(time_axis, q[3,:], 'y', label = "qz")
plt.xlabel('Time [s]')
plt.legend()
#plt.savefig('/home/hristina/Documents/Hristina/phd/Research Topics/Orientation Estimation/Quaternion Gradient Descent/figures/true_quat.eps', bbox_inches='tight')
plt.show()

# Plot absolute Euler angles
plt.plot(time_axis, EulerAngles[0,:], 'r', label = "Roll")
plt.plot(time_axis, EulerAngles[1,:], 'g', label = "Pitch")
plt.plot(time_axis, EulerAngles[2,:], 'b', label = "Yaw")
plt.legend()
plt.xlabel('Time [s]')
plt.ylabel('Angle in [$deg$]')
plt.title("Absolute Euler Angles 2")
#plt.savefig('/home/hristina/Documents/Hristina/phd/Research Topics/Orientation Estimation/Quaternion Gradient Descent/figures/abs_euler.eps', bbox_inches='tight')
plt.show()

# Plot gyro measurement
plt.plot(gyroT[1,:], 'r', label = "$\omega_x$")
plt.plot(gyroT[2,:], 'g', label = "$\omega_y$")
plt.plot(gyroT[3,:], 'b', label = "$\omega_z$")
plt.legend()
plt.xlabel('Time [s]')
plt.ylabel('$\omega [rad/s]$')
plt.title("Gyroscope Measurement")
#plt.savefig('/home/hristina/Documents/Hristina/phd/Research Topics/Orientation Estimation/Quaternion Gradient Descent/figures/abs_euler.eps', bbox_inches='tight')
plt.show()
 
