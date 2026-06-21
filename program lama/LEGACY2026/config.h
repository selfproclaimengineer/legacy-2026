#ifndef config_h
#define config_h

#include <stdint.h>

// Total jumlah servo
#define NUM_SERVOS 18

/* =======================================================
 * PETA PIN SERVO (Servo Pin Map)
 * ======================================================= */
const uint8_t SERVO_PIN_MAP[NUM_SERVOS][2] = {
    {0, 8},  {0, 9},  {0, 10}, // Kaki 0
    {0, 4},  {0, 5},  {0, 6},  // Kaki 1
    {0, 0},  {0, 1},  {0, 2},  // Kaki 2
    {1, 8},  {1, 9},  {1, 10}, // Kaki 3
    {1, 4},  {1, 5},  {1, 6},  // Kaki 4
    {1, 0},  {1, 1},  {1, 2}   // Kaki 5
};

/* =======================================================
 * DIMENSI FISIK ROBOT (mm)
 * ======================================================= */
#define COXA_LENGTH 20.0f
#define FEMUR_LENGTH 60.0f
#define TIBIA_LENGTH 80.0f

/* =======================================================
 * SETUP AWAL BODY KINEMATICS (Origins)
 * ======================================================= */
const float BODY_LEG_ORIGINS[6][3] = {
    {45.0f, 78.0f, 0.0f},  // Kaki 0 (Kanan-Depan)
    {90.0f, 0.0f, 0.0f},   // Kaki 1 (Kanan-Tengah)
    {45.0f, -78.0f, 0.0f}, // Kaki 2 (Kanan-Belakang)
    {-45.0f, -78.0f, 0.0f},// Kaki 3 (Kiri-Belakang)
    {-90.0f, 0.0f, 0.0f},  // Kaki 4 (Kiri-Tengah)
    {-45.0f, 78.0f, 0.0f}  // Kaki 5 (Kiri-Depan)
};

/* =======================================================
 * POSISI 'HOME' (BERDIRI DIAM)
 * ======================================================= */
#define HOME_X 0.0f
#define HOME_Y 80.0f
#define HOME_Z -100.0f

/* =======================================================
 * KALIBRASI SERVO RDS 3235
 * ======================================================= */
#define SERVO_PULSE_MIN 500
#define SERVO_PULSE_MAX 2500
#define SERVO_PULSE_CENTER 1500

const bool SERVO_INVERT[NUM_SERVOS] = {
    true,  false, false, // Kaki 0 (Kanan-Depan) - Disesuaikan dari logika lama Anda
    true,  false, false, // Kaki 1
    true,  false, false, // Kaki 2
    true,  true,  true,  // Kaki 3 (Kiri-Belakang)
    true,  true,  true,  // Kaki 4
    true,  true,  true   // Kaki 5
};

const float SERVO_ANGLE_OFFSET[NUM_SERVOS] = {
    0.0f,   0.0f,   0.0f,   // Kaki 0
    0.0f,   0.0f,   0.0f,   // Kaki 1
    0.0f,   0.0f,   0.0f,   // Kaki 2
    180.0f, 0.0f,   0.0f,   // Kaki 3
    180.0f, 0.0f,   0.0f,   // Kaki 4
    180.0f, 0.0f,   0.0f    // Kaki 5
};

/* =======================================================
 * PENGATURAN GERAKAN (Interpolasi)
 * ======================================================= */
#define DEFAULT_MOVE_DURATION 250 // (ms)

/* =======================================================
 * PENGATURAN GAIT (LAPIS 3) - (BARU)
 * ======================================================= */
#define GAIT_STEP_HEIGHT 40.0f   // (mm) Seberapa tinggi kaki diangkat
#define GAIT_STEP_LENGTH 60.0f   // (mm) Seberapa jauh kaki melangkah
#define GAIT_CYCLE_TIME 1000.0f  // (ms) Waktu untuk 1 langkah penuh

#endif