#ifndef HexaKinematics_h
#define HexaKinematics_h

#include <arm_math.h>
#include "config.h"
#include "HexaServos.h"
#include "LegIK.h"
#include "BodyKinematics.h"

struct Point { float x, y, z; };

class HexaKinematics {
public:
    HexaKinematics(HexaServos* servos);
    void begin();
    
    /**
     * FUNGSI UTAMA:
     * Membaca semua 'public' target state (di bawah)
     * dan menghitung seluruh pose robot.
     */
    void update();

    // --- PUSAT KONTROL (PUBLIC) ---
    Point bodyRotation;    // Target Roll, Pitch, Yaw
    Point bodyTranslation; // Target X, Y, Z
    Point legTargets[6];     // Target (x,y,z) untuk setiap UJUNG KAKI

    // --- FUNGSI HELPER (PUBLIC) ---
    uint16_t angleToPulse(uint8_t servoID, float angle);
    void setHomePose(); // Fungsi untuk reset state

private:
    HexaServos* _servos;
    Point _legOrigins[6];
    
    // --- Variabel untuk Matematika CMSIS-DSP ---
    float _T_data[16], _RX_data[16], _RY_data[16], _RZ_data[16];
    float _TempMat_data[16], _BodyTransform_data[16];
    float _vecIn_data[4], _vecOut_data[4];

    arm_matrix_instance_f32 _T, _RX, _RY, _RZ;
    arm_matrix_instance_f32 _TempMat, _BodyTransform;
    arm_matrix_instance_f32 _vecIn, _vecOut;
};

#endif