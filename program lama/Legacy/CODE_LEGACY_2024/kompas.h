#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "SparkFun_VL53L1X.h"

#define SERIAL_PORT Serial1  // Menggunakan Serial1 (RX1/TX1 pada pin 19/18)
#define buf_length 11        // Panjang buffer data yang dikirim oleh sensor

byte RxBuff[buf_length];     // Buffer untuk menyimpan data
float AngleZ = 0;            // Data sudut (angle) Z (yaw)
int dataIndex = 0;           // Menyimpan index buffer
bool startFrame = false;  

// Fungsi untuk membaca data kompas CMP10A dan mengembalikan nilai yaw
int bacaKompas() {
  unsigned long startTime = millis();  // Waktu mulai untuk timeout

  while (millis() - startTime < 1000) {  // Timeout setelah 1000 ms (1 detik)
    if (SERIAL_PORT.available()) {
      byte inputData = SERIAL_PORT.read();  // Baca byte dari sensor

      // Mendeteksi awal frame (0x55)
      if (inputData == 0x55 && !startFrame) {
        startFrame = true;
        dataIndex = 0;  // Reset index buffer
        RxBuff[dataIndex++] = inputData;
      } else if (startFrame) {
        // Masukkan data ke dalam buffer
        RxBuff[dataIndex++] = inputData;

        // Jika buffer sudah penuh, proses data
        if (dataIndex == buf_length) {
          startFrame = false;  // Reset flag frame

          // Memproses data sudut (angle) yang diterima
          if (RxBuff[1] == 0x53) {  // Data sudut (angle)
            int16_t rawAngleZ = (int16_t)(RxBuff[7] << 8 | RxBuff[6]);
            AngleZ = rawAngleZ / 32768.0 * 180;  // Konversi ke derajat (-180 hingga 180)

            // Konversi nilai sudut ke rentang 0 - 360 derajat
            if (AngleZ < 0) {
              AngleZ = 360 + AngleZ;
            }

            return AngleZ;  // Mengembalikan nilai sudut Z (yaw)
          }
        }
      }
    }
  }

  // Jika tidak ada data yang valid dalam waktu 1 detik, kembalikan -1
  return -1;
}

void ikutSelatan() {
  while (true) {
    int sudut = bacaKompas();
    
    if (sudut >= 10&& sudut <= 20) {  // Rentang untuk arah utara
      for (int i = 0; i < 3; i++) {
        passData(0);
      }
      break;
    } else if (sudut > 20 && sudut < 170) {  // Pivot kiri
      for (int i = 0; i < 3; i++) {
        passData(6);
      }
    } else if (sudut >= 170 && sudut <=360 || sudut > 0 && sudut < 10) {  // Pivot kanan
      for (int i = 0; i < 3; i++) {
        passData(5);
      }
    }
  }
  lebarLangkah = 40;
}

void ikutUtara() {
  while (true) {
    int sudut = bacaKompas();
    
    if (sudut >= 160 && sudut <= 170) {  // Rentang untuk arah selatan (180 derajat)
      for (int i = 0; i < 3; i++) {
        passData(0);
      }
      break;
    } else if (sudut > 170 && sudut <= 360) {  // Pivot kanan
      for (int i = 0; i < 3; i++) {
        passData(6);
      }
    } else if (sudut >= 0 && sudut < 160) {  // Pivot kiri
      for (int i = 0; i < 3; i++) {
        passData(5);
      }
    }
  }
  lebarLangkah = 40;
}

void ikutSelatanTangga() {
  while (true) {
    delay(50);
    int sudut = bacaKompas();
    
    if (sudut >= 180 && sudut <= 190) {  // Rentang untuk arah selatan (180 derajat)
      for (int i = 0; i < 3; i++) {
        passData(0);
      }
      break;
    } else if (sudut > 190 && sudut <= 360) {  // Pivot kanan
      for (int i = 0; i < 3; i++) {
        passData(6);
      }
    } else if (sudut >= 0 && sudut < 180) {  // Pivot kiri
      for (int i = 0; i < 3; i++) {
        passData(5);
      }
    }
  }
  lebarLangkah = 40;
}


void ikutTimur() {
  while (true) {
    int sudut = bacaKompas();
    
    if (sudut >= 80 && sudut <= 100) {  // Rentang untuk arah barat (90 derajat)
      for (int i = 0; i < 3; i++) {
        passData(0);
      }
      break;
    } else if (sudut > 100 && sudut <= 260) {  // Pivot kanan
      for (int i = 0; i < 3; i++) {
        passData(6);
      }
    } else if (sudut >= 260 || sudut < 80) {  // Pivot kiri
      for (int i = 0; i < 3; i++) {
        passData(5);
      }
    }
  }
  lebarLangkah = 40;
}

void ikutBarat() {
  while (true) {
    int sudut = bacaKompas();
    
    if (sudut >= 250 && sudut <= 260) {  // Rentang untuk arah timur (270 derajat)
      for (int i = 0; i < 3; i++) {
        passData(0);
      }
      break;
    } else if (sudut > 260 || sudut < 100) {  // Pivot kiri
      for (int i = 0; i < 3; i++) {
        passData(6);
      }
    } else if (sudut >= 100 && sudut < 250) {  // Pivot kanan
      for (int i = 0; i < 3; i++) {
        passData(5);
      }
    }
  }
  lebarLangkah = 40;
}
