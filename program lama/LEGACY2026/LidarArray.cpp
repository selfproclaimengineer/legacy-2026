#include "LidarArray.h"

LidarArray::LidarArray() {
    // Inisialisasi jarak ke 0
    for (int i = 0; i < NUM_LIDAR_SENSORS; i++) {
        _distances[i] = 0;
    }
}

void LidarArray::selectMuxChannel(uint8_t channel) {
    if (channel > 7) return;
    Wire.beginTransmission(I2C_MUX_ADDRESS);
    Wire.write(1 << channel); // Kirim byte untuk memilih channel
    Wire.endTransmission();
}

bool LidarArray::begin() {
    Wire.begin();
    
    bool all_sensors_ok = true;
    for (uint8_t i = 0; i < NUM_LIDAR_SENSORS; i++) {
        selectMuxChannel(i);
        
        // Inisialisasi sensor
        if (_distanceSensor.begin() != 0) {
            Serial.print("Sensor VL53L1X gagal di channel ");
            Serial.println(i);
            all_sensors_ok = false;
        } else {
            // Atur mode jarak (opsional, tapi disarankan)
            _distanceSensor.setDistanceModeShort(); // Mode 'Short' lebih cepat & akurat < 1.3m
            _distanceSensor.setTimingBudgetInMs(20); // 20ms, sangat cepat
        }
    }
    return all_sensors_ok;
}

void LidarArray::update() {
    // Loop melalui semua 6 sensor dan baca nilainya
    for (uint8_t i = 0; i < NUM_LIDAR_SENSORS; i++) {
        selectMuxChannel(i);
        
        // Mulai pengukuran dan tunggu (ini blocking per sensor, 
        // tapi cepat ~20ms jika timing budget diatur)
        _distanceSensor.startRanging();
        while (!_distanceSensor.checkForDataReady()) {
            delay(1);
        }
        
        // Ambil data dan simpan
        _distances[i] = _distanceSensor.getDistance();
        _distanceSensor.clearInterrupt();
        _distanceSensor.stopRanging();
    }
}

int LidarArray::getDistance(uint8_t id) {
    if (id >= NUM_LIDAR_SENSORS) return -1; // Error
    return _distances[id];
}