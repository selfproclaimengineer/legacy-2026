#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "SparkFun_VL53L1X.h"

#define lid0 cekJarak4M(0)
#define lid1 cekJarak4M(1)
#define lid2 cekJarak4M(2)
#define lid3 cekJarak4M(3)
#define lid4 cekJarak4M(4)
#define lid5 cekJarak4M(5)

// Objek sensor VL53L1X
SFEVL53L1X distanceSensor;

// Fungsi untuk memilih kanal di multiplexer I2C TCA9548A
void TCA9548A(uint8_t bus) {
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // Kirim byte untuk memilih bus
  Wire.endTransmission();
}

// Fungsi untuk membaca jarak dari sensor VL53L1X melalui multiplexer
int cekJarak4M(uint8_t lid) {
  TCA9548A(lid);
  distanceSensor.startRanging();  // Mulai pengukuran jarak
  int distance = distanceSensor.getDistance();  // Dapatkan hasil jarak
  int distance1 = kalmanFilter(distance, 1000, 200);  // Saring hasil dengan Kalman Filter
  return distance1/10;
}

// Fungsi untuk menginisialisasi sensor VL53L1X
void setupSensor() {
  Wire.begin();  // Inisialisasi I2C

  // Inisialisasi VL53L1X melalui multiplexer
  for (uint8_t i = 0; i < 6; i++) {
    TCA9548A(i);
    if (distanceSensor.begin() != 0) {
      Serial.print("Sensor VL53L1X gagal dimulai pada kanal ");
      Serial.println(i);
    }
  }

  // Setup komunikasi UART untuk CMP10A di header kompas.h
  // Panggil fungsi untuk inisialisasi kompas dari kompas.h
}

void bacaLidar() {
  Serial.print("lid0 : ");
  Serial.print(lid0);
  Serial.print("   lid1 : ");
  Serial.print(lid1);
  Serial.print("   lid2 : ");
  Serial.print(lid2);
  Serial.print("   lid3 : ");
  Serial.print(lid3);
  Serial.print("   lid4 : ");
  Serial.print(lid4);
  Serial.print("   lid5 : ");
  Serial.println(lid5);
}
// Fungsi untuk membaca status IR sensor
// int cekIr() {
//   int bacaIr = digitalRead(ir);
//   return bacaIr;
// }
