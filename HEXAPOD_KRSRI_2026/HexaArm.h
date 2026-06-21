#ifndef HEXAARM_H
#define HEXAARM_H

// Lengan/gripper untuk ambil & taruh korban (KRSRI).
// Gerak halus non-blocking antar pose (interpolasi berbasis waktu).
// ponytail: hanya kontrol pose preset (config.h), bukan IK lengan penuh.
//   Tambah IK lengan hanya jika perlu jangkau koordinat sembarang.
#include <Arduino.h>
#include "config.h"
#include "types.h"
#include "HexaServos.h"

enum ArmPose { ARM_PARK, ARM_REACH, ARM_GRIP, ARM_LIFT, ARM_DROP };

class HexaArm {
public:
    HexaArm(HexaServos* servos);
    void begin();
    void update();                       // panggil di loop
    void goTo(ArmPose pose, uint32_t durationMs = 600);
    bool isMoving() const { return _moving; }

private:
    HexaServos* _servos;
    float _cur[ARM_NUM_SERVOS];
    float _start[ARM_NUM_SERVOS];
    float _goal[ARM_NUM_SERVOS];
    uint32_t _t0, _dur;
    bool _moving;

    void writeAngle(uint8_t i, float deg);
    const float* poseData(ArmPose p);
};

#endif
