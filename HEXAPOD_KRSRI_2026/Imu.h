#ifndef IMU_H
#define IMU_H

// Pembaca IMU seri WT (WT901/JY series) via UART, frame 0x55.
// Output roll/pitch (derajat) untuk stabilisasi, yaw (0..360) untuk heading/kompas.
#include <Arduino.h>
#include "config.h"

class Imu {
public:
    Imu();
    void begin();
    void update();                 // non-blocking, panggil tiap loop

    float rollDeg()  { return _roll; }
    float pitchDeg() { return _pitch; }
    float yawDeg()   { return _yaw; }   // 0..360 (kompas)

private:
    static const int BUF = 11;
    byte _rx[BUF];
    int  _idx;
    bool _frame;
    float _roll, _pitch, _yaw;
};

#endif
