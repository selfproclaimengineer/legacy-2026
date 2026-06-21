#ifndef LEGIK_H
#define LEGIK_H

// Inverse Kinematics 1 kaki 3-DOF (coxa-femur-tibia).
// MURNI (math.h + config.h saja) -> bisa diuji di PC.
//
// Input  : (x,y,z) ujung kaki RELATIF pangkal coxa, sudah dirotasi ke
//          FRAME KAKI (neutral leg menghadap +X). z negatif = di bawah badan.
// Output : sudut geometris (derajat):
//   coxaDeg  = simpangan horizontal dari arah netral kaki
//   femurDeg = sudut femur dari bidang horizontal (+ = naik)
//   tibiaDeg = sudut interior lutut (0..180)
// Kalibrasi mekanik -> servo dilakukan di config (offset/invert), bukan di sini.

#include "config.h"
#include <math.h>

class LegIK {
public:
    // return false jika target di luar jangkauan (sudah di-clamp ke terdekat).
    static bool solve(float x, float y, float z,
                      float& coxaDeg, float& femurDeg, float& tibiaDeg);
};

#endif
