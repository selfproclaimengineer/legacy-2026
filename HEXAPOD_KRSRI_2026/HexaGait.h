#ifndef HEXAGAIT_H
#define HEXAGAIT_H

// Generator tripod gait.
//  - stance: kaki menapak, geser LURUS (kecepatan konstan) -> tak slip.
//  - swing : kaki diangkat (sin) & dikembalikan.
//  - YAW   : tiap kaki ikut komponen rotasi (cross product) -> bisa belok.
//  - profil medan: ubah tinggi/panjang langkah & tinggi badan runtime.
//
// Output -> legTargets[6] (Vec3) di FRAME BADAN, dibaca oleh Hexapod.
#include "config.h"
#include "types.h"

struct GaitProfile {
    float stepHeight;   // mm
    float stepLength;   // mm
    float cycleTime;    // ms
    float standHeight;  // mm (foot z = -standHeight)
};

class HexaGait {
public:
    HexaGait();
    void begin();
    void update();                       // hitung legTargets
    void setMoveVector(float vx, float vy, float vyaw); // -1..1 strafe, maju, putar
    void setProfile(const GaitProfile& p) { _prof = p; }
    GaitProfile profile() const { return _prof; }

    Vec3 legTargets[6];

private:
    GaitProfile _prof;
    Vec3 _footHome[6];      // posisi netral ujung kaki (frame badan)
    float _vx, _vy, _vyaw;
    bool _running;
    unsigned long _cycleStart;
    void computeHome();
};

#endif
