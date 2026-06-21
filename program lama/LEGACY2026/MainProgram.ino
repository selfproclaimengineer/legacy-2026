#include "Hexapod.h"
#include "IMUReader.h"
#include "LidarArray.h"
#include "FuzzyController.h" // <--- 1. IMPOR FUZZY

Hexapod robot;
IMUReader imu;
LidarArray lidar;
FuzzyController fuzzy(&robot, &lidar); // <--- 2. BUAT OBJEK FUZZY

void setup() {
  Serial.begin(115200);
  Serial.println("Booting Hexapod (Fuzzy Mode)...");
  
  robot.begin(); 
  imu.begin();   
  lidar.begin();
  fuzzy.begin(); // <--- 3. INISIALISASI FUZZY
  
  delay(2000); 
  // Kita tidak perlu panggil robot.walk() lagi, Fuzzy yang urus
}

void loop() {
  // --- 1. BACA SEMUA SENSOR ---
  imu.update();
  lidar.update(); 

  // --- 2. JALANKAN ALGORITMA ---
  
  // Algoritma Stabilisasi
  robot.kinematics->bodyRotation.x = -imu.getRoll();
  robot.kinematics->bodyRotation.y = -imu.getPitch();
  
  // Algoritma Penghindar (FUZZY)
  // Ini akan menghitung & memanggil robot.walk()
  fuzzy.update(); // <--- 4. GANTIKAN if..else DENGAN INI
  
  // --- 3. JALANKAN ROBOT ---
  robot.update();
}