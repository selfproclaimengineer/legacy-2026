#include "LidarArray.h"

LidarArray::LidarArray() {
    for (int i = 0; i < NUM_LIDAR; i++) _dist[i] = -1;
    _cur = 0;
    _ranging = false;
}

void LidarArray::selectMux(uint8_t ch) {
    LIDAR_I2C_BUS.beginTransmission(I2C_MUX_ADDR);
    LIDAR_I2C_BUS.write(1 << ch);
    LIDAR_I2C_BUS.endTransmission();
}

bool LidarArray::begin() {
    LIDAR_I2C_BUS.begin();
    LIDAR_I2C_BUS.setClock(LIDAR_I2C_CLOCK);
    bool ok = true;
    for (uint8_t i = 0; i < NUM_LIDAR; i++) {
        selectMux(i);
        if (_sensor.begin() != 0) {
            Serial.print("VL53L1X gagal channel "); Serial.println(i);
            ok = false;
        } else {
            _sensor.setDistanceModeShort();   // < ~1.3m, cepat & akurat
            _sensor.setTimingBudgetInMs(20);
        }
    }
    return ok;
}

// State machine: tiap panggilan -> mulai ranging sensor _cur, atau ambil hasil bila siap.
void LidarArray::update() {
    selectMux(_cur);
    if (!_ranging) {
        _sensor.startRanging();
        _ranging = true;
        return;            // beri waktu sensor mengukur (cek lagi panggilan berikut)
    }
    if (_sensor.checkForDataReady()) {
        int mm = _sensor.getDistance();
        _sensor.clearInterrupt();
        _sensor.stopRanging();
        _ranging = false;

        int cm = mm / 10;
        // low-pass sederhana (EMA). ponytail: cukup; Kalman penuh hanya jika perlu.
        if (cm >= 0 && cm <= 400) {
            _dist[_cur] = (_dist[_cur] < 0) ? cm : (int)(0.6f * _dist[_cur] + 0.4f * cm);
        }
        _cur = (_cur + 1) % NUM_LIDAR;   // lanjut sensor berikut
    }
    // jika belum siap: keluar, coba lagi loop berikutnya (tanpa blocking)
}

int LidarArray::getDistance(uint8_t id) {
    if (id >= NUM_LIDAR) return -1;
    return _dist[id];
}
