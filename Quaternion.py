#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  8 18:02:59 2021

@author: Hristina
"""
import numpy as np
import math
class Quaternion:
    def EulerAngles_to_Quaternion(alpha, beta, gamma):
        qw = np.cos(alpha/2.0) * np.cos(beta/2.0) * np.cos(gamma/2.0) + np.sin(alpha/2.0) * np.sin(beta/2.0) * np.sin(gamma/2.0)
        qx = np.sin(alpha/2.0) * np.cos(beta/2.0) * np.cos(gamma/2.0) - np.cos(alpha/2.0) * np.sin(beta/2.0) * np.sin(gamma/2.0)
        qy = np.cos(alpha/2.0) * np.sin(beta/2.0) * np.cos(gamma/2.0) + np.sin(alpha/2.0) * np.cos(beta/2.0) * np.sin(gamma/2.0)
        qz = np.cos(alpha/2.0) * np.cos(beta/2.0) * np.sin(gamma/2.0) - np.sin(alpha/2.0) * np.sin(beta/2.0) * np.cos(gamma/2.0)
        
        return np.array([qw, qx, qy, qz], dtype = np.float)
    
    def Tait_Bryan_to_Quaternion(roll, pitch, yaw):
        c1 = np.cos(-yaw/2.0)
        c2 = np.cos(-pitch / 2.0)
        c3 = np.cos( roll / 2.0)
        c12 = c1 * c2
        s1 = np.sin(-yaw / 2.0)
        s2 = np.sin(-pitch / 2.0)
        s3 = np.sin( roll / 2.0)
        s12 = s1 * s2
        
        qw = c1 * s2 * c3 - s1 * c2 * s3
        qx = c12 * s3 + s12 * c3
        qy = s1 * c2 * c3 + c1 * s2 * s3
        qz = c12 * c3 - s12 * s3
        return [qw, qx, qy, qz]
    
    def Quaternion_product(quaternion1, quaternion0):
        qw0, qx0, qy0, qz0 = quaternion0
        qw1, qx1, qy1, qz1 = quaternion1
#        qw = -qx1 * qx0 - qy1 * qy0 - qz1 * qz0 + qw1 * qw0
#        qx = qx1 * qw0 + qy1 * qz0 - qz1 * qy0 + qw1 * qx0
#        qy = -qx1 * qz0 + qy1 * qw0 + qz1 * qx0 + qw1 * qy0
#        qz = qx1 * qy0 - qy1 * qx0 + qz1 * qw0 + qw1 * qz0
#        return np.quaternion(qw, qx, qy, qz)
        return np.array([-qx1 * qx0 - qy1 * qy0 - qz1 * qz0 + qw1 * qw0,
                         qx1 * qw0 + qy1 * qz0 - qz1 * qy0 + qw1 * qx0,
                         -qx1 * qz0 + qy1 * qw0 + qz1 * qx0 + qw1 * qy0,
                         qx1 * qy0 - qy1 * qx0 + qz1 * qw0 + qw1 * qz0], dtype = np.float)
    
    def Quaternion_Rotation_Matrix(q):
        # Extract the values from Q
        qw = q[0]
        qx = q[1]
        qy = q[2]
        qz = q[3]
         
        # First row of the rotation matrix
        r00 = 2 * (qw * qw + qx * qx) - 1
        r01 = 2 * (qx * qy - qw * qz)
        r02 = 2 * (qx * qz + qw * qy)
         
        # Second row of the rotation matrix
        r10 = 2 * (qx * qy + qw * qz)
        r11 = 2 * (qw * qw + qy * qy) - 1
        r12 = 2 * (qy * qz - qw * qx)
         
        # Third row of the rotation matrix
        r20 = 2 * (qx * qz - qw * qy)
        r21 = 2 * (qy * qz + qw * qx)
        r22 = 2 * (qw * qw + qz * qz) - 1
         
        # 3x3 rotation matrix
        rot_matrix = np.array([[r00, r01, r02],
                               [r10, r11, r12],
                               [r20, r21, r22]])
                                
        return rot_matrix
    
    def to_euler(q):
        w = q[0]
        x = q[1]
        y = q[2]
        z = q[3]

        ysqr = y * y
    
        t0 = +2.0 * (w * x + y * z)
        t1 = +1.0 - 2.0 * (x * x + ysqr)
        X = math.degrees(math.atan2(t0, t1))
    
        t2 = +2.0 * (w * y - z * x)
        t2 = +1.0 if t2 > +1.0 else t2
        t2 = -1.0 if t2 < -1.0 else t2
        Y = math.degrees(math.asin(t2))
    
        t3 = +2.0 * (w * z + x * y)
        t4 = +1.0 - 2.0 * (ysqr + z * z)
        Z = math.degrees(math.atan2(t3, t4))
    
        return X, Y, Z
    
    def to_conjugate(q):
        qw = q[0]
        qx = -q[1]
        qy = -q[2]
        qz = -q[3]
        
        return [qw, qx, qy, qz]

      

        