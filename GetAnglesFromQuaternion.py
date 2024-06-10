import numpy as np
import math

def GetAnglesFromQuaternion (q):
    q0 = q[0]  
    q1 = q[1]  
    q2 = q[2]  
    q3 = q[3]  

    roll = np.degrees(np.arctan2(2*q2*q3-2*q0*q1, 2*(q0*q0 + q3*q3)-1))
    if roll < -160:
        roll = roll + 360
#	pitch = -np.degrees(np.arcsin(2*q2*q0+2*q1*q3))
    yaw = np.degrees(np.arctan2(2*q1*q2-2*q0*q3,2*(q0*q0)+2*(q1*q1)-1))


    t0 = +2.0 * (q3 * q2 - q1 * q0)
    t1 = 2.0 * (q1 * q1 + q0 * q0) - 1.0
#	roll = np.degrees(np.arctan2(t0, t1))
    t2 = +2.0 * (q3 * q1 - q2 * q0)
    t2 = +1.0 if t2 > +1.0 else t2
    t2 = -1.0 if t2 < -1.0 else t2
    pitch = math.degrees(math.asin(t2))
    t3 = +2.0 * (q0 * q3 + q1 * q2)
    t4 = +1.0 - 2.0 * (q3 * q3 + q2 * q2)
#	yaw = math.degrees(math.atan2(t3, t4))
    
    Angles = [roll,pitch,yaw]
    return (Angles)

#def GetAnglesFromQuaternion (q):
#    q0 = q[0]  
#    q1 = q[1]  
#    q2 = q[2]  
#    q3 = q[3]  
#    
#    t0 = +2.0 * (q3 * q2 + q1 * q0)
#    t1 = 1.0 - 2.0 * (q1 * q1 + q2 * q2)
#    roll = np.arctan2(t0, t1)
#    
#    t2 = +2.0 * (q3 * q1 - q2 * q0)
##    t2 = +1.0 if t2 > +1.0 else t2
##    t2 = -1.0 if t2 < -1.0 else t2
#    pitch = math.asin(t2)
#    
#    t3 = +2.0 * (q0 * q3 + q1 * q2)
#    t4 = +1.0 - 2.0 * (q3 * q3 + q2 * q2)
#    yaw = math.atan2(t3, t4)
#    
#    Angles = [roll,pitch,yaw]
#    return (Angles)
