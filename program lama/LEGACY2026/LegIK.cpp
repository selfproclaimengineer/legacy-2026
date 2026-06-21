#include "LegIK.h"
#include <math.h> // Impor library matematika standar C++

namespace {
    // Helper function (internal) untuk konversi radian ke derajat
    // M_PI adalah konstanta Pi yang presisi dari math.h
    float rad2deg(float rad) {
        return (rad * 180.0f / M_PI);
    }
}

// Implementasi fungsi 'solve'
void LegIK::solve(float x, float y, float z,
                   float &coxaAngle, float &femurAngle, float &tibiaAngle) 
{
    float degree1, degree2, degree3;
    float degree2_1, degree2_2;
    float L1, L, L_2, Lcox;
    float temp2, temp3;
    float femur_2, tibia_2;

    // Gunakan fungsi math.h standar (presisi tinggi & akselerasi FPU)
    degree1 = atan2(y, x);
    L1 = sqrt((x * x) + (y * y));
    Lcox = L1 - COXA_LENGTH;
    L = sqrt((z * z) + (Lcox * Lcox));
    femur_2 = FEMUR_LENGTH * FEMUR_LENGTH;
    tibia_2 = TIBIA_LENGTH * TIBIA_LENGTH;
    L_2 = L * L;

    degree2_1 = atan2(z, Lcox);
    
    // Hitung temp2 (argumen untuk acos)
    temp2 = (((L_2 + femur_2) - tibia_2) / ((2 * FEMUR_LENGTH) * L));
    
    // Safety check (Domain acos adalah -1.0 s/d 1.0)
    // Mencegah error 'NaN' (Not-a-Number) jika kaki stretch berlebihan
    if (temp2 > 1.0f) temp2 = 1.0f;
    else if (temp2 < -1.0f) temp2 = -1.0f;
    
    degree2_2 = acos(temp2);
    degree2 = degree2_2 + degree2_1;

    // Hitung temp3 (argumen untuk acos)
    temp3 = (((femur_2 + tibia_2) - L_2) / ((2 * FEMUR_LENGTH) * TIBIA_LENGTH));
    
    // Safety check
    if (temp3 > 1.0f) temp3 = 1.0f;
    else if (temp3 < -1.0f) temp3 = -1.0f;
    
    degree3 = acos(temp3);

    // Konversi hasil akhir ke derajat
    coxaAngle = rad2deg(degree1);
    femurAngle = rad2deg(degree2);
    tibiaAngle = rad2deg(degree3);

    // Terapkan koreksi dari kode lama Anda
    if (x < 0 && y < 0) coxaAngle = coxaAngle + 360;
    if (L <= (FEMUR_LENGTH + TIBIA_LENGTH)) tibiaAngle = tibiaAngle - 90;
}