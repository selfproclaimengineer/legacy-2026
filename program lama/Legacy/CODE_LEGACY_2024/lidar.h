#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "SparkFun_VL53L1X.h"

// Definisi untuk memanggil fungsi cekJarak4M
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
  Wire.beginTransmission(0x70);  // Alamat TCA9548A adalah 0x70
  Wire.write(1 << bus);          // Pilih kanal dengan bit shift
  Wire.endTransmission();
}



// Fungsi untuk membaca jarak dari sensor VL53L1X melalui multiplexer
int cekJarak4M(uint8_t lid) {
  TCA9548A(lid);
  distanceSensor.startRanging();
  int distance = distanceSensor.getDistance();  // Mengambil data jarak (milimeter)
  distanceSensor.stopRanging();

  // Konversi ke centimeter
  float distanceCm = distance / 10.0;

  // Filter dengan Kalman Filter
  float filteredDistance = kalmanFilter(distanceCm, 100, 10);

  // Validasi hasil jarak (0 - 400 cm)
  if (filteredDistance < 0 || filteredDistance > 400) {
    return -1;  // Nilai -1 jika hasil tidak valid
  }

  return (int)filteredDistance;
}

// Inisialisasi sensor VL53L1X
void setupSensor() {
  Wire.begin();
  Wire.setClock(400000);
  // Inisialisasi multiplexer dan sensor
  for (uint8_t i = 0; i < 6; i++) {
    TCA9548A(i);
    if (distanceSensor.begin() != 0) {
      Serial.print("Sensor VL53L1X gagal dimulai pada kanal ");
      Serial.println(i);
    } else {
      Serial.print("Sensor VL53L1X berhasil dimulai pada kanal ");
      Serial.println(i);
    }
  }
}

// Fungsi untuk membaca dan menampilkan hasil jarak dari semua sensor
void bacaLidar() {
  Serial.print("lid0: "); Serial.print(lid0); Serial.print(" cm");
  Serial.print("   lid1: "); Serial.print(lid1); Serial.print(" cm");
  Serial.print("   lid2: "); Serial.print(lid2); Serial.print(" cm");
  Serial.print("   lid3: "); Serial.print(lid3); Serial.print(" cm");
  Serial.print("   lid4: "); Serial.print(lid4); Serial.print(" cm");
  Serial.print("   lid5: "); Serial.println(lid5); Serial.print(" cm");
}


