#ifndef LegIK_h
#define LegIK_h

#include "config.h"   // Dibutuhkan untuk COXA_LENGTH, FEMUR_LENGTH, TIBIA_LENGTH
#include <Arduino.h>  // Dibutuhkan untuk sqrt dan math.h

class LegIK {
public:
    /**
     * Menghitung Inverse Kinematics (IK) untuk kaki 3-DOF.
     * @param x Posisi X target (relatif terhadap pangkal coxa)
     * @param y Posisi Y target (relatif terhadap pangkal coxa)
     * @param z Posisi Z target (relatif terhadap pangkal coxa)
     * @param coxaAngle Referensi untuk menyimpan hasil sudut Coxa (derajat)
     * @param femurAngle Referensi untuk menyimpan hasil sudut Femur (derajat)
     * @param tibiaAngle Referensi untuk menyimpan hasil sudut Tibia (derajat)
     */
    static void solve(float x, float y, float z,
                      float &coxaAngle, float &femurAngle, float &tibiaAngle);
};

#endif