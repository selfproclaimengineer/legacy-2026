import math
import time
# import varspeed
from adafruit_servokit import ServoKit

kit = ServoKit(channels=16)

BASE_HGT = 25
HUMERUS = 103
ULNA = 93
GRIPPER = 140

def ftl(x):
    return int(x + 0.5) if x >= 0 else int(x - 0.5)

hum_sq = HUMERUS * HUMERUS
uln_sq = ULNA * ULNA

loopCounter = 0
pulseWidth = 6.6
microsecondsToDegrees = 0
    
def map(val, min1, max1, min2, max2):
  y = (((max2-min2)/(max1-min1))*(val-min1)) + min2
  return y

def set_arm(x, y, z, grip_angle_d):
    grip_angle_r = math.radians(grip_angle_d)
    
    bas_angle_r = math.atan2(x, y)
    rdist = math.sqrt((x * x) + (y * y))
    
    y = rdist
    
    grip_off_z = (math.sin(grip_angle_r)) * GRIPPER
    grip_off_y = (math.cos(grip_angle_r)) * GRIPPER
    
    wrist_z = (z - grip_off_z) - BASE_HGT
    wrist_y = y - grip_off_y
    
    s_w = (wrist_z * wrist_z) + (wrist_y * wrist_y)
    s_w_sqrt = math.sqrt(s_w)
    
    a1 = math.atan2(wrist_z, wrist_y)
    
    a2 = math.acos(((hum_sq - uln_sq) + s_w) / (2 * HUMERUS * s_w_sqrt))
    
    shl_angle_r = a1 + a2
    shl_angle_d = math.degrees(shl_angle_r)
    
    elb_angle_r = math.acos((hum_sq + uln_sq - s_w) / (2 * HUMERUS * ULNA))
    elb_angle_d = math.degrees(elb_angle_r)
    elb_angle_dn = -(180.0 - elb_angle_d)
    
    wri_angle_d = (grip_angle_d - elb_angle_dn) - shl_angle_d
    
    bas_servopulse = 1500.0 - ((math.degrees(bas_angle_r)) * pulseWidth)
    shl_servopulse = 1500.0 + ((shl_angle_d - 90.0) * pulseWidth)
    elb_servopulse = 1500.0 - ((elb_angle_d - 90.0) * pulseWidth)
    
    wri_servopulse = 1500 - (wri_angle_d * pulseWidth)
    
    microsecondsToDegrees = map(ftl(bas_servopulse), 544, 2400, 0, 180)
    kit.servo[0].angle = microsecondsToDegrees
    
    microsecondsToDegrees = map(ftl(shl_servopulse), 544, 2400, 0, 180)
    kit.servo[1].angle = microsecondsToDegrees
    
    microsecondsToDegrees = map(ftl(elb_servopulse), 544, 2400, 0, 180)
    kit.servo[2].angle = microsecondsToDegrees
    
    microsecondsToDegrees = map(ftl(wri_servopulse), 544, 2400, 0, 180)
    kit.servo[3].angle = microsecondsToDegrees
    kit.servo[4].angle = 0
    kit.servo[5].angle = 0

def servo_park():
    kit.servo[0].angle = 90
    kit.servo[1].angle = 90
    kit.servo[2].angle = 90
    kit.servo[3].angle = 90
    kit.servo[4].angle = 90
    return

def zero_x():
    for yaxis in range(250, 401):
        print(" yaxis= : ", yaxis)
        set_arm(0, yaxis, 200.0, 0)
        time.sleep(0.01)
    for yaxis in range(400, 249, -1):
        set_arm(0, yaxis, 200.0, 0)
        time.sleep(0.01)

def line():
    for xaxis in range(-100, 101, 2):
        set_arm(xaxis, 100, 120, 0)
        time.sleep(0.01)
    for xaxis in range(100, -101, -2):
        set_arm(xaxis, 100, 120, 0)
        time.sleep(0.01)

def bentukL() :
    for xaxis in range(-100, 101, 1):
        set_arm(xaxis, 100, 120, 0)
        time.sleep(0.008)
    for i in range(0, 50, 1):
        kit.servo[4].angle = i
        time.sleep(0.02)
    for i in range(0, 40, 1):
        kit.servo[5].angle = i
        time.sleep(0.02)
    for zaxis in range(120, 210, 1):
        set_arm(xaxis, 100, zaxis, 0)
        time.sleep(0.008)
    for i in range(50, 100, 1):
        kit.servo[4].angle = i
        time.sleep(0.02)
def circle():
    RADIUS = 50.0
    
    for angle in range(0, 360):
        yaxis = RADIUS * math.sin(math.radians(angle)) + 300
        zaxis = RADIUS * math.cos(math.radians(angle)) + 200
        set_arm(0, yaxis, zaxis, 0, 50)
        time.sleep(0.01)

print("Start")
time.sleep(2)
while True:
    bentukL()	
        

