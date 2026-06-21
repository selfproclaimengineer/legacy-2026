import numpy as np
import pygame
import sys
from adafruit_servokit import ServoKit
import time
import RPi.GPIO as GPIO
import threading
import serial

# Inisialisasi komunikasi serial
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

# Set mode pin GPIO
GPIO.setmode(GPIO.BCM)

# Definisikan pin untuk sensor jarak ultrasonik
SRF_PINS = [17, 22, 10, 9, 11]

# Panjang segmen lengan robot (untuk visualisasi dalam pygame)
L1 = 100  # Panjang segmen bahu
L2 = 100  # Panjang segmen siku
L3 = 50   # Panjang segmen gripper

# Inisialisasi modul PCA9685 untuk mengendalikan servo
kit = ServoKit(channels=16, frequency=50)

# Inisialisasi sudut awal untuk setiap sendi
angle_base = 90
angle_shoulder = 180
angle_elbow = 90
angle_gripper = 0

# Variabel untuk menyimpan nilai jarak dari sensor ultrasonik
distances = [0] * len(SRF_PINS)

# Fungsi untuk membaca jarak dari sensor ultrasonik
def read_distance(pin, index):
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(pin, GPIO.LOW)
    
    GPIO.setup(pin, GPIO.IN)
    pulse_start = time.time()
    while GPIO.input(pin) == 0:
        pulse_start = time.time()
        
    while GPIO.input(pin) == 1:
        pulse_end = time.time()
        
    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)

    # Simpan nilai jarak ke dalam list distances
    distances[index] = distance
    
    return distance

# Fungsi untuk mengirim data melalui serial
def send_data(data):
    ser.write(data.encode())
    print(f"Data sent: {data}")

# Fungsi untuk memulai thread pembacaan sensor
def start_sensor_reading(pin, index):
    while True:
        distance = read_distance(pin, index)
        print(f"Sensor {pin}: {distance} cm")
        time.sleep(1)

# Fungsi untuk mengubah sudut menjadi nilai PWM
def angle_to_pwm(angle):
    return int((angle / 180.0) * 4096.0)

# Fungsi untuk memindahkan servo secara mulus
def move_servo_smooth(channel, target_angle, speed=1.0, delay=0.02):
    current_angle = kit.servo[channel].angle

    if current_angle is None:
        current_angle = 0

    direction = 1 if target_angle > current_angle else -1
    steps = int(abs(target_angle - current_angle) / speed)

    for _ in range(steps):
        current_angle += speed * direction
        kit.servo[channel].angle = current_angle
        time.sleep(delay)

# Inisialisasi Pygame
pygame.init()

# Atur ukuran layar
screen_width, screen_height = 800, 600
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption('Arm Bot Control')

# Warna
white = (255, 255, 255)
black = (0, 0, 0)
gray = (169, 169, 169)

# Font untuk teks
font = pygame.font.Font(None, 36)

# Fungsi untuk menggambar segmen lengan robot
def draw_segment(x, y, length, angle):
    end_x = x + length * np.cos(np.radians(angle))
    end_y = y - length * np.sin(np.radians(angle))
    pygame.draw.line(screen, black, (x, y), (end_x, end_y), 5)
    return end_x, end_y

# Loop utama
running = True
speed_factor = 1.0  # Faktor kecepatan, bisa disesuaikan
while running:
    # Mulai thread untuk membaca sensor
    threads = []
    for i, pin in enumerate(SRF_PINS):
        thread = threading.Thread(target=start_sensor_reading, args=(pin, i))
        thread.daemon = True
        thread.start()
        threads.append(thread)
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Dapatkan input keyboard
    keys = pygame.key.get_pressed()

    # Kontrol servo dasar (base)
    if keys[pygame.K_q]:
        angle_base = max(0, angle_base - 5 * speed_factor)
        angle_base = min(180, angle_base)
    if keys[pygame.K_w]:
        angle_base = min(180, angle_base + 5 * speed_factor)
        angle_base = max(0, angle_base)

    # Kontrol servo bahu
    if keys[pygame.K_e]:
        angle_shoulder = max(0, angle_shoulder - 5 * speed_factor)
        angle_shoulder = min(180, angle_shoulder)
    if keys[pygame.K_r]:
        angle_shoulder = min(180, angle_shoulder + 5 * speed_factor)
        angle_shoulder = max(0, angle_shoulder)

    # Kontrol servo siku
    if keys[pygame.K_t]:
        angle_elbow = max(0, angle_elbow - 5 * speed_factor)
        angle_elbow = min(180, angle_elbow)
    if keys[pygame.K_y]:
        angle_elbow = min(180, angle_elbow + 5 * speed_factor)
        angle_elbow = max(0, angle_elbow)

    # Kontrol gripper
    if keys[pygame.K_o]:
        angle_gripper = max(0, angle_gripper - 5 * speed_factor)
        angle_gripper = min(30, angle_gripper)
    if keys[pygame.K_p]:
        angle_gripper = min(30, angle_gripper + 5 * speed_factor)
        angle_gripper = max(0, angle_gripper)

    # Kirim perintah gerak ke robot melalui serial
    if keys[pygame.K_a]:
        send_data('A')
        print("Move Forward")
    elif keys[pygame.K_d]:
        send_data('D')
        print("Move Backward")

    # Perbarui posisi servo
    move_servo_smooth(0, angle_base, speed=speed_factor)
    move_servo_smooth(1, angle_shoulder, speed=speed_factor)
    move_servo_smooth(2, angle_elbow, speed=speed_factor)
    move_servo_smooth(3, angle_gripper, speed=speed_factor)

    # Hapus layar
    screen.fill(white)

    # Gambar lingkaran di sekitar base servo
    pygame.draw.circle(screen, gray, (screen_width // 2, screen_height // 2), 30)

    # Hitung posisi akhir gripper
    end_shoulder_x, end_shoulder_y = draw_segment(screen_width // 2, screen_height // 2, L1, angle_base)
    end_elbow_x, end_elbow_y = draw_segment(end_shoulder_x, end_shoulder_y, L2, angle_shoulder)
    end_gripper_x, end_gripper_y = draw_segment(end_elbow_x, end_elbow_y, L3, angle_elbow)

    # Gambar gripper
    pygame.draw.rect(screen, black, (end_gripper_x - 15, end_gripper_y - 5, 30, 10))

    # Tampilkan sudut setiap sendi
    text_base = font.render(f"Base: {angle_base} deg", True, black)
    text_shoulder = font.render(f"Shoulder: {angle_shoulder} deg", True, black)
    text_elbow = font.render(f"Elbow: {angle_elbow} deg", True, black)
    text_gripper = font.render(f"Gripper: {angle_gripper} deg", True, black)

    screen.blit(text_base, (10, 10))
    screen.blit(text_shoulder, (10, 50))
    screen.blit(text_elbow, (10, 90))
    screen.blit(text_gripper, (10, 130))

    # Tampilkan nilai jarak dari sensor ultrasonik
    for i, distance in enumerate(distances):
        text_sensor = font.render(f"Sensor {i + 1}: {distance} cm", True, black)
        screen.blit(text_sensor, (10, 170 + i * 40))

    # Tampilkan kontrol keyboard
    text_controls = font.render("Controls: Q/W - Base | E/R - Shoulder | T/Y - Elbow | O/P - Gripper | A/D - Move", True, black)
    screen.blit(text_controls, (10, screen_height - 40))

    pygame.display.flip()

    # Tunggu sebentar untuk memberikan waktu layar dirender dan mengurangi kecepatan gerakan
    pygame.time.delay(50)

# Keluar dari aplikasi Pygame dan tutup program
pygame.quit()
sys.exit()
