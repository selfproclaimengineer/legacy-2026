#ifndef LidarArray_h
#define LidarArray_h

#include <Wire.h>
#include "SparkFun_VL53L1X.h"

#define NUM_LIDAR_SENSORS 6
#define I2C_MUX_ADDRESS 0x70

class LidarArray {
public:
    LidarArray();

    /**
     * @brief Menginisialisasi I2C Mux dan semua 6 sensor LiDAR.
     * @return true jika semua sensor terdeteksi, false jika gagal.
     */
    bool begin();

    /**
     * @brief Membaca semua 6 sensor secara berurutan.
     * Panggil fungsi ini satu kali di dalam loop() utama Anda.
     */
    void update();

    /**
     * @brief Mendapatkan jarak terakhir yang dibaca dari sensor.
     * @param id Sensor (0-5).
     * @return Jarak dalam milimeter (mm).
     */
    int getDistance(uint8_t id);

private:
    /**
     * @brief Fungsi helper untuk memilih channel I2C pada Mux.
     */
    void selectMuxChannel(uint8_t channel);

    // Satu objek sensor (kita pindah-pindah channel-nya)
    SFEVL53L1X _distanceSensor;

    // Array untuk menyimpan 6 nilai jarak (mm)
    int _distances[NUM_LIDAR_SENSORS];
};

#endif