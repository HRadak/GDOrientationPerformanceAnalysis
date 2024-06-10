#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  8 16:37:11 2021

@author: Hristina
"""
import matplotlib.pyplot as plt
import numpy as np
from Brownian import Brownian
from Quaternion import Quaternion
from WriteData import WriteData
import math

n_size = 50000 #how much data in total
m_size = 10000 #data for no movement
EulerAngles = np.zeros((3, n_size))
RelativeAngles = np.zeros((3, n_size))
euler_in_deg = np.zeros((3, n_size))

gyro = np.zeros((4, n_size))
acc = np.zeros((4, n_size))
mag = np.zeros((4, n_size))

noise_gyro = np.zeros((4, n_size))
noise_acc = np.zeros((4, n_size))

q0 = np.zeros((4,1))
acc_r = np.zeros((4,1))
mag_r = np.zeros((4,1))

acc_r[3,0] = -1
mag_r[2,0] = 1

q_rel = np.zeros((4, 1))

q = np.zeros((4, n_size))
q_gyro = np.zeros((4, n_size))
q_acc = np.zeros((4, n_size))

q[0,0] = 1
q_gyro[0,0] = 1

b = Brownian()
#Generate noise for acc and gyro, generate absolute change in orientation Euler Angles in units of Radian
for i in range(3):
    noise_gyro[i+1,:] = b.gen_awgn(n_size, 0.05)
    noise_acc[i+1,:] = b.gen_awgn(n_size, 0.005)
    EulerAngles[0,:] = np.concatenate([b.gen_data(m_size, 0), b.gen_data(n_size-m_size, 0.3, False)]) #absolute Euler Angles
    euler_in_deg[i,:] = EulerAngles[i,:]*180/math.pi
    plt.plot(euler_in_deg[i,:])
plt.title("Euler Angles")
plt.show()

#compute relative change in orientation
for i in range(n_size-1):
    RelativeAngles[:,i] = EulerAngles[:,i+1] - EulerAngles[:,i]
    q_rel = Quaternion.EulerAngles_to_Quaternion(RelativeAngles[0,i],RelativeAngles[1,i],RelativeAngles[2,i])
    #Calculate the quaternion from Euler Angles
    q[:,i+1] = Quaternion.Quaternion_product(q[:,i], q_rel)
    
plt.title("Quaternion from Euler Angles")
plt.plot(q[0,:], 'r', label = "qw")
plt.plot(q[1,:], 'g', label = "qx")
plt.plot(q[2,:], 'b', label = "qy")
plt.plot(q[3,:], 'y', label = "qz")
plt.show()

#compute relative change in orientation
for i in range(n_size-1):
    q_rel = Quaternion.Quaternion_product( Quaternion.to_conjugate(q[:,i+1]), q[:,i])
    RelativeAngles[:,i] = EulerAngles[:,i+1] - EulerAngles[:,i]
    
#calculate gyroscope measuerments   
for i in range(n_size):
    gyro[1:4,i] = RelativeAngles[:,i] * (100,100,100)# + noise_gyro[1:4,i] #Ts sampling time = 0.01s = 10ms
#recalculate quaternion from gyroscope measuerments
for i in range(n_size-1):
    q0 = q_gyro[:, i] + 1/200*Quaternion.Quaternion_product(q_gyro[:, i], gyro[:,i])
    q_gyro[:,i+1] = q0/np.linalg.norm(q0)
   
plt.title("Quaternion from angular velocity")
plt.plot(q_gyro[0,:], 'r', label = "qw1")
plt.plot(q_gyro[1,:], 'g', label = "qx1")
plt.plot(q_gyro[2,:], 'b', label = "qy1")
plt.plot(q_gyro[3,:], 'y', label = "qz1")
plt.show()

plt.title("Difference")
diff = []
diff = abs(np.subtract(q, q_gyro))
for i in range (4):
    plt.plot(diff[i,:])
plt.show()

#calculate accelerometer measuerments
for i in range(n_size):
    q0 = Quaternion.Quaternion_product(Quaternion.to_conjugate((q[:,i].reshape(4,1))), acc_r)
    acc[:,i] = Quaternion.Quaternion_product(q0.reshape(4,), q[:,i])# + noise_acc[:,i]
plt.title("Quaternion acc")
plt.plot(acc[0,:], 'r', label = "qw")
plt.plot(acc[1,:], 'g', label = "qx")
plt.plot(acc[2,:], 'b', label = "qy")
plt.plot(acc[3,:], 'y', label = "qz")
#plt.show()    
#calculate magnetometer (equivalent) measuerments
for i in range(n_size):
    q0 = Quaternion.Quaternion_product(Quaternion.to_conjugate((q[:,i].reshape(4,1))), mag_r)
    mag[:,i] = Quaternion.Quaternion_product(q0.reshape(4,), q[:,i]) + noise_acc[:,i]
plt.title("Quaternion mag")
plt.plot(mag[0,:], 'r', label = "qw")
plt.plot(mag[1,:], 'g', label = "qx")
plt.plot(mag[2,:], 'b', label = "qy")
plt.plot(mag[3,:], 'y', label = "qz")
plt.show()   
#Save synthetic data to .dat files
lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[GYRO_DATA]']
with open('gyro.dat', 'w') as f:
    f.write('\n'.join(lines))
   
with open('gyro.dat', 'a+') as f:
    f.write("\n")
    f.write("<gyro_x>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (gyro[1,n]))
    f.write("</gyro_x>")
    f.write("\n")
        
    f.write("<gyro_y>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (gyro[2,n]))
    f.write("</gyro_y>")
    f.write("\n")

    f.write("<gyro_z>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (gyro[3,n]))
    f.write("</gyro_z>")
    
lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[ACC_DATA]']
with open('acc.dat', 'w') as f:
    f.write('\n'.join(lines))
   
with open('acc.dat', 'a+') as f:
    f.write("\n")
    f.write("<acc_x>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (acc[1,n]))
    f.write("</acc_x>")
    f.write("\n")
        
    f.write("<acc_y>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (acc[2,n]))
    f.write("</acc_y>")
    f.write("\n")

    f.write("<acc_z>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (acc[3,n]))
    f.write("</acc_z>")
    
lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[MAG_DATA]']
with open('mag.dat', 'w') as f:
    f.write('\n'.join(lines))
   
with open('mag.dat', 'a+') as f:
    f.write("\n")
    f.write("<mag_x>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (mag[1,n]))
    f.write("</mag_x>")
    f.write("\n")
        
    f.write("<mag_y>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (mag[2,n]))
    f.write("</mag_y>")
    f.write("\n")

    f.write("<mag_z>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (mag[3,n]))
    f.write("</mag_z>")
    
lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[QUAT_DATA]']
with open('quat.dat', 'w') as f:
    f.write('\n'.join(lines))
   
with open('quat.dat', 'a+') as f:
    f.write("\n")
    f.write("<quat_w>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (q[0,n]))
    f.write("</quat_w>")
    f.write("\n")
      
    f.write("<quat_x>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (q[1,n]))
    f.write("</quat_y>")
    f.write("\n")
    
    f.write("<quat_y>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (q[2,n]))
    f.write("</quat_y>")
    f.write("\n")

    f.write("<quat_z>*********(double)\n")
    for n in range(n_size):
        f.write("%s\n" % (q[3,n]))
    f.write("</quat_z>")
print("Data saved into .dat files")  
