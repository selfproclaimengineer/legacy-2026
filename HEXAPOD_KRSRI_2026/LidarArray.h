#ifndef LIDARARRAY_H
#define LIDARARRAY_H

// 6x VL53L1X via mux TCA9548A, NON-BLOCKING (state machine round-robin).
// update() memajukan SATU sensor per panggilan tanpa busy-wait -> loop tetap 50Hz.
// Nilai dalam cm, sudah di-filter low-pass. getDistance() = nilai terakhir.
#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h"
#include "config.h"

class LidarArray {
public:
    LidarArray();
    bool begin();
    void update();                 // non-blocking
    int  getDistance(uint8_t id);  // cm (terakhir valid), -1 jika error

private:
    SFEVL53L1X _sensor;
    int  _dist[NUM_LIDAR];
    uint8_t _cur;        // sensor yang sedang diproses
    bool _ranging;       // sudah startRanging?
    void selectMux(uint8_t ch);
};

#endif
