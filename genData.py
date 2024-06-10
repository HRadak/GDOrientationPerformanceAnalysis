
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
import math
from GetAnglesFromQuaternion import *
from scipy.spatial.transform import Rotation as R
import sys

#folder_plots = "./Results/plots/"
#folder_plots = "/home/hristina/Documents/Hristina/radak2023a/figures/sensor_data_plots_stacked/
def runSimulator(i):
    n_size = 10000 #how much data in total
    sampling_time = 0.01 #in seconds
    time_axis = []
    var_g = np.zeros((3,1), dtype = np.double)
    b_var_g = np.zeros((3,1), dtype = np.double)
    
    var_g = [0.004803871527954,	0.00099743344257, 0.003393434907676] #sigma_omega noise variance gyro
    b_var_g[:,0] = [0.005,0.005,0.005]
    var_a = [0.060752930685806,	0.010105633537792, 0.052844574242773] #sigma_a noise variance acc
    var_m = [0.248161184191773, 0.948849794812692, 0.574174198795116] #sigma_m noise variance mag
    
    cov_mat_g_b = np.zeros((3,3), dtype = np.double)
    cov_mat_g_n = np.zeros((3,3), dtype = np.double)
    
    np.fill_diagonal(cov_mat_g_b, b_var_g)
    np.fill_diagonal(cov_mat_g_n, var_g)
    
    M_acc = np.zeros((3,3), dtype = np.double)
    M_ = np.zeros((3,3), dtype = np.double)
    
    M_acc[:,0] = [0.98,0.003,0.003]
    M_acc[:,1] = [0.003,0.97,0.003]
    M_acc[:,2] = [0.003,0.003,1.02]
    
    gyro = np.zeros((4, n_size), dtype = np.double)
    gyroT = np.zeros((4, n_size), dtype = np.double)
    acc = np.zeros((4, n_size), dtype = np.double)
    mag = np.zeros((4, n_size), dtype = np.double)
    acc2 = np.zeros((4, n_size), dtype = np.double)
    accT = np.zeros((4, n_size), dtype = np.double)
    magT = np.zeros((4, n_size), dtype = np.double)
    
    b_gyro = np.zeros((4, n_size), dtype = np.double) # TODO check the precision required
    b_acc = [] #constant
    b_mag = [] #constant
    
    noise_gyro = np.zeros((4, n_size), dtype = np.double)
    noise_acc = np.zeros((4, n_size), dtype = np.double)
    noise_mag = np.zeros((4, n_size), dtype = np.double)
    unit_noise_1 = np.zeros((3, n_size), dtype = np.double)
    unit_noise_2 = np.zeros((3, n_size), dtype = np.double)
    
    EulerAngles = np.zeros((3, n_size))
    relEulerAngles = np.zeros((3, n_size))
    euler_in_deg = np.zeros((3, n_size))
    
    q_dot = np.zeros((4, n_size), dtype = np.double)
    q_rel = np.zeros((4, n_size), dtype = np.double)
    q = np.zeros((4, n_size), dtype = np.double)
    q2 = np.zeros((4, n_size), dtype = np.double)
    q_gyro = np.zeros((4, n_size), dtype = np.double)
    
    
    q0 = np.zeros((4,1))
    acc_r = np.zeros((4,1))
    mag_r = np.zeros((4,1))
    
    acc_r[3,0] = -1
    #mag_r[2,0] = -1
    mag_r[:,0] = [0,0.391801903,0,0.920049601] #another reference vec for mag if the inclination is known
    
    #q[0,0] = 1
    #q2[0,0] = 1
    q[:,0] = [1,0,0,0]
    q2[:,0] = [1,0,0,0]
    q_gyro[0,0] = 1
    
    b = Brownian()
    for i in range(n_size):
        time_axis.append(0.01*i)
    
    for i in range(3):
        #Generate noise for acc and gyro
        noise_acc[i+1,:] = b.gen_awgn(n_size, var_a[i])
        noise_mag[i+1,:] = b.gen_awgn(n_size, var_m[i])
        noise_gyro[i+1,:] = b.gen_awgn(n_size, var_g[i])
    
        unit_noise_1[i,:] = b.gen_awgn(n_size,1)
        unit_noise_2[i,:] = b.gen_awgn(n_size,1)
        
        #Generate absolute Euler Angles
    for i in range(3):
        EulerAngles[i,:] = np.concatenate([b.gen_steady_data(n_size)]) #True for absolute Euler Angles
    #    EulerAngles[i,:] = np.concatenate([np.sin(np.linspace(0, np.pi, 1500)), np.linspace(0,0,2000), np.sin(np.linspace(-np.pi, 3*np.pi, 1500))])
    #    EulerAngles[i,:] = np.concatenate([np.linspace(0,0,10000), np.sin(np.linspace(0, np.pi, 1500)), np.linspace(0,0,1000), np.sin(np.linspace(-np.pi, 3*np.pi, 1500))])
    #    EulerAngles[i,:] = np.concatenate([np.linspace(0,0,9000), np.linspace(0,0,1000)])
    
        #TODO comment/umcomment noise_gyro calc
        #generate gyro noise eq 4.54a Markley
        for j in range(n_size):
            noise_gyro[i+1,j] = math.sqrt(b_var_g[i]*b_var_g[i]/sampling_time + 1/12*var_g[i]*var_g[i]*sampling_time)
            noise_gyro[i+1,j] = math.sqrt(noise_gyro[i+1,j])*unit_noise_2[i,j]
                  
    for i in range(n_size):
        euler_in_deg[:,i] = EulerAngles[:,i]*180/math.pi
     
    for i in range(n_size-1):
        b_gyro[1:4,i+1] = np.transpose(b_var_g).dot(unit_noise_1[:,i].reshape(3,1))
    #    b_gyro[1:4,i+1] = np.transpose(b_var_g).dot(cov_mat_g_b)
        b_gyro[:,i+1] *= math.sqrt(sampling_time)
        b_gyro[:,i+1] += b_gyro[:,i]
    
    for i in range(n_size):
        b_acc = [0,0.1,-0.05,-0.13] # constant value 0,70 mg from Datasheet
        b_mag = [0,22,12,23] # TODO check constant value
    

    
    #Calc. relative Euler Angles
    for i in range(0, n_size-1):
        relEulerAngles[:,i] = EulerAngles[:,i+1] - EulerAngles[:,i]
    for i in range(n_size):
        q_rel[:,i] = Quaternion.EulerAngles_to_Quaternion(relEulerAngles[0,i],relEulerAngles[1,i],relEulerAngles[2,i])
    
    # Calculate true gyroscope measurement
    for i in range(n_size):
        gyroT[1:4,i] = relEulerAngles[:,i]/sampling_time #rad/s
        gyro[1:4,i] = relEulerAngles[:,i]/sampling_time #rad/s
    
    # Calculate true quaternion from true gyro measurement
    for i in range(n_size-1):
        q_dot[:,i] = 0.5*Quaternion.Quaternion_product(q[:,i],gyro[:,i])
        q[:,i+1] = q[:,i] + q_dot[:,i]*0.01 #sampling time = 0.01s
        q[:,i+1] = q[:,i+1]/np.linalg.norm(q[:,i+1])
        
#    for i in range(n_size):
#        EulerAngles[:,i] = GetAnglesFromQuaternion(q[:,i])
           
#    # Calculate true quaternion from absolute orientation
#    for i in range(n_size-1):
#    #    q2[:,i+1] = Quaternion.EulerAngles_to_Quaternion(EulerAngles[0,i+1],EulerAngles[1,i+1],EulerAngles[2,i+1])
#        q2[:,i+1] = Quaternion.Quaternion_product(q_rel[:,i],q2[:,i])# TODO testing
#        q2[:,i+1] = q2[:,i+1]/np.linalg.norm(q2[:,i+1])
        
    # Calculate gyroscope with noise
    for i in range(n_size):
        if i>0:
    #        gyro[:,i] = b_gyro[:,i] + b_gyro[:,i-1]
            M_ = M_acc
            gyro_ = M_.dot(gyro[1:4,i])
            gyro[1:4,i] = gyro_
            gyro[:,i] += 0.5*(b_gyro[:,i] + b_gyro[:,i-1]) + noise_gyro[:,i]
        else: 
            gyro[:,i] = gyro[:,i] + b_gyro[:,i] + noise_gyro[:,i]

    #calculate accelerometer measuerments
    # acc = q_(k)* x acc_r x q_(k); acc_r vector rotated by quaternion q_(k) 
    for i in range(n_size):
        q0 = Quaternion.Quaternion_product(Quaternion.to_conjugate((q[:,i].reshape(4,1))), acc_r)
        acc[:,i] = Quaternion.Quaternion_product(q0.reshape(4,), q[:,i])
        acc[:,i] *= 9.81
        accT[:,i] = acc[:,i]
        acc2[1:4,i] = M_acc.dot(acc[1:4,i])
        acc[:,i] = acc2[:,i]
        acc[:,i] += b_acc
        acc[:,i] += noise_acc[:,i]
         
    #calculate magnetometer measuerments
    for i in range(n_size):
        q0 = Quaternion.Quaternion_product(Quaternion.to_conjugate((q[:,i].reshape(4,1))), mag_r)
        mag[:,i] = Quaternion.Quaternion_product(q0.reshape(4,), q[:,i])
        mag[:,i] *= 65
        M_ = M_acc
        magT[:,i] = mag[:,i]
        mag[1:4,i] =M_.dot(magT[1:4,i])
        mag[:,i] += b_mag
        mag[:,i] += noise_mag[:,i]
    
    #Save synthetic data to .dat files   
    if (iteration<10):
        iteration_str = "000"
    elif (iteration<100):
        iteration_str = "00"
    elif (iteration<1000):
        iteration_str = "0"

    iteration_str +=str(iteration)

    lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[GYRO_DATA]']
    with open('./SyntheticData2/gyro_' + iteration_str + '.dat', 'w') as f:
        f.write('\n'.join(lines))
       
    with open('./SyntheticData2/gyro_' + iteration_str + '.dat', 'a+') as f:
        f.write("\n")
        f.write("<gyro_x>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (gyroT[1,n]))
        f.write("</gyro_x>")
        f.write("\n")
            
        f.write("<gyro_y>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (gyroT[2,n]))
        f.write("</gyro_y>")
        f.write("\n")
    
        f.write("<gyro_z>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (gyroT[3,n]))
        f.write("</gyro_z>")
        
    lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[ACC_DATA]']
    with open('./SyntheticData2/acc_' + iteration_str + '.dat', 'w') as f:
        f.write('\n'.join(lines))
       
    with open('./SyntheticData2/acc_' + iteration_str + '.dat', 'a+') as f:
        f.write("\n")
        f.write("<acc_x>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (accT[1,n]))
        f.write("</acc_x>")
        f.write("\n")
            
        f.write("<acc_y>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (accT[2,n]))
        f.write("</acc_y>")
        f.write("\n")
    
        f.write("<acc_z>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (accT[3,n]))
        f.write("</acc_z>")
        
    lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[ACC_DATA]']
    with open('./SyntheticData2/acc_ideal_' + iteration_str + '.dat', 'w') as f:
        f.write('\n'.join(lines))
       
    with open('./SyntheticData2/acc_ideal_' + iteration_str + '.dat', 'a+') as f:
        f.write("\n")
        f.write("<acc_x>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (accT[1,n]))
        f.write("</acc_x>")
        f.write("\n")
            
        f.write("<acc_y>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (accT[2,n]))
        f.write("</acc_y>")
        f.write("\n")
    
        f.write("<acc_z>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (accT[3,n]))
        f.write("</acc_z>")
        
    lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[MAG_DATA]']
    with open('./SyntheticData2/mag_' + iteration_str + '.dat', 'w') as f:
        f.write('\n'.join(lines))
       
    with open('./SyntheticData2/mag_' + iteration_str + '.dat', 'a+') as f:
        f.write("\n")
        f.write("<mag_x>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (magT[1,n]))
        f.write("</mag_x>")
        f.write("\n")
            
        f.write("<mag_y>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (magT[2,n]))
        f.write("</mag_y>")
        f.write("\n")
    
        f.write("<mag_z>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (magT[3,n]))
        f.write("</mag_z>")
        
    lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[MAG_DATA]']
    with open('./SyntheticData2/mag_ideal_' + iteration_str + '.dat', 'w') as f:
        f.write('\n'.join(lines))
       
    with open('./SyntheticData2/mag_ideal_' + iteration_str + '.dat', 'a+') as f:
        f.write("\n")
        f.write("<mag_x>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (magT[1,n]))
        f.write("</mag_x>")
        f.write("\n")
            
        f.write("<mag_y>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (magT[2,n]))
        f.write("</mag_y>")
        f.write("\n")
    
        f.write("<mag_z>*********(double)\n")
        for n in range(n_size):
            f.write("%s\n" % (magT[3,n]))
        f.write("</mag_z>")
        
    lines = ['[GLOBAL_DATA]', '<numSamples>', str(n_size), '</numSamples>', '[QUAT_DATA]']
    with open('./SyntheticData2/quat_' + iteration_str + '.dat', 'w') as f:
        f.write('\n'.join(lines))
       
    with open('./SyntheticData2/quat_' + iteration_str + '.dat', 'a+') as f:
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

    with open('./SyntheticData2/sense_' + iteration_str + '.cfg', 'w') as f:
        f.write('Mode = steady\n')
        f.write('DataSource = SyntheticData\n')
        f.write('GyroData = gyro_'+ iteration_str  + '.dat\n')
        f.write('AccData = acc_'+ iteration_str  + '.dat\n')
        f.write('MagData = mag_'+ iteration_str  + '.dat\n')
        f.write('QuatData = quat_'+ iteration_str  + '.dat\n')
        f.write('AccData_ideal = acc_ideal_'+ iteration_str  + '.dat\n')
        f.write('MagData_ideal = mag_ideal_'+ iteration_str  + '.dat\n')

        f.write('QuatDataResult = quatResult_'+ iteration_str + '\n')
        f.write('EulerDataResult = eulerResult_'+ iteration_str + '\n')
    
if __name__ == '__main__':
    num_iterations = sys.argv[1]
    for iteration in range(int(num_iterations)):
        print("Iterator ", iteration)
        runSimulator(iteration)
