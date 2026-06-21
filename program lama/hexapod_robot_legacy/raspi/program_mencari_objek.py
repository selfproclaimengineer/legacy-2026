import cv2
import numpy as np
import math
from adafruit_servokit import ServoKit

# Inisialisasi modul PCA9685
pca = ServoKit(channels=16)

# Konfigurasi batas sudut servo untuk masing-masing sendi lengan robot
servo_limits = {
    'base': [0, 180],
    'shoulder': [0, 180],
    'elbow': [0, 180],
    'wrist_pitch': [0, 180],
    'wrist_roll': [0, 180],
    'grip': [0, 180]
}

# Nilai sudut awal untuk setiap servo
initial_servo_angles = {
    'base': 90,
    'shoulder': 50,
    'elbow': 90,
    'wrist_pitch': 50,
    'wrist_roll': 125,
    'grip': 45
}

low_orange = np.array([2, 155, 150])
high_orange = np.array([12, 255, 255])

# Variabel untuk smoothing
smoothing_factor = 0.9
smoothed_angles = {
    'base': 90,
    'shoulder': 50,
    'elbow': 90,
    'wrist_pitch': 50,
    'wrist_roll': 125,
    'grip': 45
}

def obj_data(img):
    obj_width = 0
    obj_center = (0, 0)
    
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, low_orange, high_orange)
    _, mask1 = cv2.threshold(mask, 254, 255, cv2.THRESH_BINARY)
    
    cnts, _ = cv2.findContours(mask1, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    for c in cnts:
        if len(cnts) > 0:
            largest_contour = max(cnts, key=cv2.contourArea)
            x, y, w, h = cv2.boundingRect(largest_contour)
            obj_width = w
            obj_center = (x + w // 2, y + h // 2)
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)

    return obj_center, obj_width

def calculate_servo_angles(obj_center, img_width, img_height, speed=0.1):
    global smoothed_angles
    
    delta_x = obj_center[0] - img_width // 2
    delta_y = obj_center[1] - img_height // 2
    delta_z = 0  # Sesuaikan sesuai kebutuhan
    
    # Gerakan awal: Cari objek dengan base servo saja
    smoothed_angles['base'] = smoothing_factor * smoothed_angles['base'] + (1 - smoothing_factor) * 90
    pca.servo[0].angle = int(smoothed_angles['base'])
    
    if obj_center != (0, 0):  # Jika objek terdeteksi
        # Base Servo (Yaw)
        angle_base = math.atan2(delta_y, delta_x) * (180 / math.pi)
        angle_base = max(min(angle_base, servo_limits['base'][1]), servo_limits['base'][0])
        smoothed_angles['base'] = smoothing_factor * smoothed_angles['base'] + (1 - smoothing_factor) * angle_base
        pca.servo[0].angle = int(smoothed_angles['base'])

        # Shoulder Servo (Pitch)
        angle_shoulder = math.atan2(delta_z, math.sqrt(delta_x**2 + delta_y**2)) * (180 / math.pi)
        angle_shoulder = max(min(angle_shoulder, servo_limits['shoulder'][1]), servo_limits['shoulder'][0])
        smoothed_angles['shoulder'] = smoothing_factor * smoothed_angles['shoulder'] + (1 - smoothing_factor) * angle_shoulder
        pca.servo[1].angle = int(smoothed_angles['shoulder'])

        # Elbow Servo
        angle_elbow = 90 - angle_shoulder
        angle_elbow = max(min(angle_elbow, servo_limits['elbow'][1]), servo_limits['elbow'][0])
        smoothed_angles['elbow'] = smoothing_factor * smoothed_angles['elbow'] + (1 - smoothing_factor) * angle_elbow
        pca.servo[2].angle = int(smoothed_angles['elbow'])

        # Wrist Pitch Servo
        angle_wrist_pitch = math.atan2(delta_y, delta_x) * (180 / math.pi) - angle_base
        angle_wrist_pitch = max(min(angle_wrist_pitch, servo_limits['wrist_pitch'][1]), servo_limits['wrist_pitch'][0])
        smoothed_angles['wrist_pitch'] = smoothing_factor * smoothed_angles['wrist_pitch'] + (1 - smoothing_factor) * angle_wrist_pitch
        pca.servo[3].angle = int(smoothed_angles['wrist_pitch'])

        # Wrist Roll Servo (Selalu di 125 derajat dan dihentikan)
        angle_wrist_roll = 125
        pca.servo[4].angle = int(angle_wrist_roll)

        # Grip Servo
        angle_grip = f(obj_width)
        angle_grip = max(min(angle_grip, servo_limits['grip'][1]), servo_limits['grip'][0])
        smoothed_angles['grip'] = smoothing_factor * smoothed_angles['grip'] + (1 - smoothing_factor) * angle_grip
        pca.servo[5].angle = int(smoothed_angles['grip'])

    return smoothed_angles['base'], smoothed_angles['shoulder'], smoothed_angles['elbow'], smoothed_angles['wrist_pitch'], smoothed_angles['wrist_roll'], smoothed_angles['grip']

# Contoh fungsi f(delta_obj_width) untuk sudut grip
def f(delta_obj_width):
    # Implementasi fungsi sesuai kebutuhan
    return delta_obj_width * (25/100)  # Sesuaikan dengan nilai sudut yang diinginkan

# Gantilah dengan inisialisasi webcam yang sesuai
webcam = cv2.VideoCapture(0)

while True:
    _, frame = webcam.read()
    
    obj_center, obj_width = obj_data(frame)
    angles = calculate_servo_angles(obj_center, frame.shape[1], frame.shape[0], speed=0.05)  # Sesuaikan speed sesuai kebutuhan

    cv2.imshow("Robot Arm Control", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

webcam.release()
cv2.destroyAllWindows()
