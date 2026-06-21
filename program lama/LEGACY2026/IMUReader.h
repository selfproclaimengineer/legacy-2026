#ifndef IMUReader_h
#define IMUReader_h

#include <Arduino.h>
#include <math.h>

#define SERIAL_PORT Serial1
#define IMU_BAUD 9600
#define BUF_LENGTH 11

class IMUReader {
public:
    IMUReader();
    void begin();
    void update();

    float getRoll() { return _roll; }
    float getPitch() { return _pitch; }
    float getYaw() { return _yaw; }

private:
    byte _RxBuff[BUF_LENGTH];
    int _dataIndex;
    bool _startFrame;

    float _roll, _pitch, _yaw;
};

#endif