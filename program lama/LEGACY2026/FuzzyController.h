#ifndef FuzzyController_h
#define FuzzyController_h

#include "LidarArray.h"
#include "Hexapod.h"

// Kita akan definisikan input dan output kita
// Ini adalah "Himpunan Fuzzy" (Fuzzy Sets)
enum FuzzDist { DEKAT, AMAN, JAUH };
enum FuzzSide { KIRI_OK, KIRI_DEKAT, KANAN_OK, KANAN_DEKAT };
enum FuzzOut { MUNDUR, MAJU_PELAN, MAJU_PENUH, BELOK_KIRI, BELOK_KANAN };

class FuzzyController {
public:
    FuzzyController(Hexapod* robot, LidarArray* lidar);
    
    // Panggil ini di setup()
    void begin();
    
    // Panggil ini di loop() utama
    void update();

private:
    // Pointers ke hardware yang kita kontrol/baca
    Hexapod* _robot;
    LidarArray* _lidar;

    // --- Langkah 1: Fuzzifikasi ---
    // Variabel untuk menyimpan 'derajat keanggotaan' (0.0 - 1.0)
    float _in_depan_dekat, _in_depan_aman;
    float _in_kiri_aman, _in_kiri_dekat;
    float _in_kanan_aman, _in_kanan_dekat;

    // Fungsi helper untuk Fuzzifikasi
    void fuzzifyInputs();
    float trapezoid(float x, float a, float b, float c, float d);
    float triangle(float x, float a, float b, float c);

    // --- Langkah 2 & 3: Inferensi & Defuzzifikasi ---
    void runRules();
};

#endif