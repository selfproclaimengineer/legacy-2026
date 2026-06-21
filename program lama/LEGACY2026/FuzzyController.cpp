#include "FuzzyController.h"

// --- BATASAN UNTUK FUZZIFIKASI (dalam mm) ---
// Sesuaikan angka-angka ini untuk mengubah perilaku robot
const float DEPAN_DEKAT_START = 0;
const float DEPAN_DEKAT_END = 200;
const float DEPAN_AMAN_START = 150;
const float DEPAN_AMAN_END = 500;

const float SAMPING_DEKAT_START = 0;
const float SAMPING_DEKAT_END = 150;
const float SAMPING_AMAN_START = 100;
const float SAMPING_AMAN_END = 400;

// --- KEKUATAN OUTPUT (Bobot) ---
// Seberapa kuat robot harus bereaksi
const float OUT_MUNDUR = -1.0f; // y_speed
const float OUT_MAJU_PELAN = 0.3f; // y_speed
const float OUT_MAJU_PENUH = 1.0f; // y_speed
const float OUT_BELOK_KIRI = 0.8f; // yaw_speed
const float OUT_BELOK_KANAN = -0.8f; // yaw_speed


FuzzyController::FuzzyController(Hexapod* robot, LidarArray* lidar) {
    _robot = robot;
    _lidar = lidar;
}

void FuzzyController::begin() {
    // Tidak ada setup khusus yang diperlukan untuk saat ini
}

void FuzzyController::update() {
    // 1. FUZZIFIKASI: Ubah jarak (mm) menjadi nilai fuzzy (0.0 - 1.0)
    fuzzifyInputs();
    
    // 2. INFERENSI & DEFUZZIFIKASI: Jalankan aturan
    runRules();
}

void FuzzyController::fuzzifyInputs() {
    // Ambil data sensor
    int d_depan = _lidar->getDistance(0); // Asumsi 0 = Depan
    int d_kiri1 = _lidar->getDistance(2);
    int d_kiri2 = _lidar->getDistance(3);
    int d_kanan1 = _lidar->getDistance(4);
    int d_kanan2 = _lidar->getDistance(5);
    
    // Ambil rata-rata sensor samping
    int d_kiri = (d_kiri1 + d_kiri2) / 2;
    int d_kanan = (d_kanan1 + d_kanan2) / 2;

    // Fuzzifikasi Input Depan
    _in_depan_dekat = trapezoid(d_depan, DEPAN_DEKAT_START, DEPAN_DEKAT_START, DEPAN_DEKAT_END, DEPAN_AMAN_START);
    _in_depan_aman = trapezoid(d_depan, DEPAN_DEKAT_END, DEPAN_AMAN_START, DEPAN_AMAN_END, DEPAN_AMAN_END);
    
    // Fuzzifikasi Input Samping
    _in_kiri_dekat = trapezoid(d_kiri, SAMPING_DEKAT_START, SAMPING_DEKAT_START, SAMPING_DEKAT_END, SAMPING_AMAN_START);
    _in_kiri_aman = trapezoid(d_kiri, SAMPING_DEKAT_END, SAMPING_AMAN_START, SAMPING_AMAN_END, SAMPING_AMAN_END);
    
    _in_kanan_dekat = trapezoid(d_kanan, SAMPING_DEKAT_START, SAMPING_DEKAT_START, SAMPING_DEKAT_END, SAMPING_AMAN_START);
    _in_kanan_aman = trapezoid(d_kanan, SAMPING_DEKAT_END, SAMPING_AMAN_START, SAMPING_AMAN_END, SAMPING_AMAN_END);
}

void FuzzyController::runRules() {
    // --- Variabel Aturan (Kekuatan setiap aturan, 0.0 - 1.0) ---
    float rule1, rule2, rule3, rule4;
    
    // --- RULE BASE (Aturan Fuzzy) ---
    
    // Aturan 1: JIKA Depan SANGAT DEKAT, MAKA MUNDUR.
    rule1 = _in_depan_dekat;
    
    // Aturan 2: JIKA Depan AMAN (tidak dekat), MAKA MAJU PENUH.
    // Kita gunakan (1.0 - _in_depan_dekat)
    rule2 = 1.0f - _in_depan_dekat; 
    
    // Aturan 3: JIKA Kiri DEKAT, MAKA BELOK KANAN.
    rule3 = _in_kiri_dekat;
    
    // Aturan 4: JIKA Kanan DEKAT, MAKA BELOK KIRI.
    rule4 = _in_kanan_dekat;

    // --- DEFUZZIFIKASI (Metode Rata-rata Tertimbang) ---
    
    float totalWeight = rule1 + rule2 + rule3 + rule4;
    
    // Hindari pembagian dengan nol
    if (totalWeight < 0.01) {
        _robot->stop();
        return;
    }
    
    // Hitung output untuk y_speed (Maju/Mundur)
    float y_speed = ( (rule1 * OUT_MUNDUR) + (rule2 * OUT_MAJU_PENUH) ) / (rule1 + rule2);
    
    // Hitung output untuk yaw_speed (Belok)
    float yaw_speed = ( (rule3 * OUT_BELOK_KANAN) + (rule4 * OUT_BELOK_KIRI) ) / (rule3 + rule4);

    // Atasi jika salah satu pembagi (rule3+rule4) adalah nol
    if (isnan(yaw_speed)) yaw_speed = 0.0f;
    
    // Kirim perintah akhir ke robot
    _robot->walk(y_speed, 0.0f, yaw_speed); // (y, x, yaw)
}


// --- Fungsi Keanggotaan (Membership Functions) ---

// Fungsi Trapesium
float FuzzyController::trapezoid(float x, float a, float b, float c, float d) {
    if (x <= a || x >= d) return 0.0f;
    if (x >= b && x <= c) return 1.0f;
    if (x > a && x < b) return (x - a) / (b - a);
    if (x > c && x < d) return (d - x) / (d - c);
    return 0.0f;
}

// Fungsi Segitiga (tidak terpakai, tapi bagus untuk dimiliki)
float FuzzyController::triangle(float x, float a, float b, float c) {
    return trapezoid(x, a, b, b, c);
}