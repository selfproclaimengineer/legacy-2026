#ifndef HexaServos_h
#define HexaServos_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h" // Mengimpor file kalibrasi

#define PCA9685_1_ADDR 0x40
#define PCA9685_2_ADDR 0x41
#define SERVO_FREQ 50 // 50 Hz update rate

class HexaServos {
public:
    // Constructor (meminta pointer ke peta pin)
    HexaServos(const uint8_t (*pin_map)[2]);
    
    // Inisialisasi driver
    void begin();
    
    // Set target pulse (microseconds) untuk satu servo
    void setPulse(uint8_t servoID, uint16_t pulse);
    
    // Fungsi non-blocking, harus dipanggil di loop()
    // Ini akan mengirim 18 sinyal setiap 20ms
    void commit(); 

private:
    Adafruit_PWMServoDriver kit0 = Adafruit_PWMServoDriver(PCA9685_1_ADDR);
    Adafruit_PWMServoDriver kit1 = Adafruit_PWMServoDriver(PCA9685_2_ADDR);

    // Pointer untuk menyimpan alamat dari peta pin
    const uint8_t (*_pin_map)[2];

    // Array untuk menyimpan 18 target pulse
    uint16_t targetPulses[NUM_SERVOS];

    // Variabel untuk timing 50Hz
    unsigned long lastUpdateTime;
    const unsigned int updateInterval = 20; // 20 ms
};

#endif