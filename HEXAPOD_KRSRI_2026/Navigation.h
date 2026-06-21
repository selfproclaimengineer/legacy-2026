#ifndef NAVIGATION_H
#define NAVIGATION_H

// Helper navigasi closed-loop (header-only, inline).
// Menghasilkan perintah gerak {forward, strafe, turn} dari sensor.
// FSM misi yang memanggil robot.walk(...) tiap loop -> non-blocking.
#include "config.h"
#include "types.h"

struct NavCmd { float forward, strafe, turn; };

namespace Nav {

    // Belok di tempat menuju heading target (derajat kompas).
    inline NavCmd holdHeading(float yawNow, float yawTarget) {
        float err = angleDiffDeg(yawTarget, yawNow);
        float turn = clampf(HEADING_KP * err, -1.0f, 1.0f);
        return { 0.0f, 0.0f, turn };
    }
    inline bool atHeading(float yawNow, float yawTarget) {
        return fabsf(angleDiffDeg(yawTarget, yawNow)) <= HEADING_TOL_DEG;
    }

    // Maju sambil ikut dinding KANAN, jaga heading. front/right dalam cm.
    inline NavCmd followWallRight(float yawNow, float yawTarget, int frontCm, int rightCm) {
        if (frontCm >= 0 && frontCm < FRONT_STOP_CM)
            return { 0.0f, 0.0f, 0.6f };                 // depan mentok -> putar kiri
        float head = HEADING_KP * angleDiffDeg(yawTarget, yawNow);
        // turn>0 = CCW(kiri). Terlalu jauh dari dinding kanan -> belok kanan (turn<0).
        float wall = (rightCm >= 0) ? -WALL_KP * (float)(rightCm - WALL_SETPOINT_CM) : 0.0f;
        float turn = clampf(head + wall, -1.0f, 1.0f);
        return { NAV_FWD_SPEED, 0.0f, turn };
    }

    // Maju sambil ikut dinding KIRI.
    inline NavCmd followWallLeft(float yawNow, float yawTarget, int frontCm, int leftCm) {
        if (frontCm >= 0 && frontCm < FRONT_STOP_CM)
            return { 0.0f, 0.0f, -0.6f };                // depan mentok -> putar kanan
        float head = HEADING_KP * angleDiffDeg(yawTarget, yawNow);
        // Terlalu jauh dari dinding kiri -> belok kiri (turn>0).
        float wall = (leftCm >= 0) ? WALL_KP * (float)(leftCm - WALL_SETPOINT_CM) : 0.0f;
        float turn = clampf(head + wall, -1.0f, 1.0f);
        return { NAV_FWD_SPEED, 0.0f, turn };
    }

    // Maju lurus jaga heading saja (tanpa dinding).
    inline NavCmd goStraight(float yawNow, float yawTarget) {
        float turn = clampf(HEADING_KP * angleDiffDeg(yawTarget, yawNow), -1.0f, 1.0f);
        return { NAV_FWD_SPEED, 0.0f, turn };
    }
}

#endif
