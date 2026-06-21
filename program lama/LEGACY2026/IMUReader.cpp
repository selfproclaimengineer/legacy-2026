#include "IMUReader.h"

IMUReader::IMUReader() {
    _dataIndex = 0;
    _startFrame = false;
    _roll = 0.0f;
    _pitch = 0.0f;
    _yaw = 0.0f;
}

void IMUReader::begin() {
    SERIAL_PORT.begin(IMU_BAUD);
}

void IMUReader::update() {
    while (SERIAL_PORT.available()) {
        byte inputData = SERIAL_PORT.read();

        if (inputData == 0x55 && !_startFrame) {
            _startFrame = true;
            _dataIndex = 0;
            _RxBuff[_dataIndex++] = inputData;
        } 
        else if (_startFrame) {
            _RxBuff[_dataIndex++] = inputData;

            if (_dataIndex == BUF_LENGTH) {
                _startFrame = false; 

                if (_RxBuff[1] == 0x53) {
                    int16_t rawRoll = (int16_t)(_RxBuff[3] << 8 | _RxBuff[2]);
                    int16_t rawPitch = (int16_t)(_RxBuff[5] << 8 | _RxBuff[4]);
                    int16_t rawYaw = (int16_t)(_RxBuff[7] << 8 | _RxBuff[6]);

                    _roll = (rawRoll / 32768.0f) * M_PI;
                    _pitch = (rawPitch / 32768.0f) * M_PI;
                    _yaw = (rawYaw / 32768.0f) * M_PI;
                }
            }
        }
    }
}