#include "Imu.h"

Imu::Imu() { _idx = 0; _frame = false; _roll = _pitch = _yaw = 0; }

void Imu::begin() { IMU_SERIAL.begin(IMU_BAUD); }

void Imu::update() {
    while (IMU_SERIAL.available()) {
        byte b = IMU_SERIAL.read();
        if (b == 0x55 && !_frame) { _frame = true; _idx = 0; _rx[_idx++] = b; }
        else if (_frame) {
            _rx[_idx++] = b;
            if (_idx == BUF) {
                _frame = false;
                if (_rx[1] == 0x53) {   // paket sudut
                    int16_t r = (int16_t)(_rx[3] << 8 | _rx[2]);
                    int16_t p = (int16_t)(_rx[5] << 8 | _rx[4]);
                    int16_t y = (int16_t)(_rx[7] << 8 | _rx[6]);
                    _roll  = r / 32768.0f * 180.0f;
                    _pitch = p / 32768.0f * 180.0f;
                    float yaw = y / 32768.0f * 180.0f;
                    if (yaw < 0) yaw += 360.0f;   // 0..360 untuk kompas
                    _yaw = yaw;
                }
            }
        }
    }
}
