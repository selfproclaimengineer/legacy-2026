#include "HexaGait.h"
#include <Arduino.h>

HexaGait::HexaGait() {
    _prof = { GAIT_STEP_HEIGHT, GAIT_STEP_LENGTH, GAIT_CYCLE_TIME, STAND_HEIGHT };
    _vx = _vy = _vyaw = 0;
    _running = false;
    _cycleStart = 0;
}

void HexaGait::computeHome() {
    // Kaki netral = pangkal coxa + STAND_RADIUS searah hadap kaki.
    for (int i = 0; i < 6; i++) {
        float a = deg2rad(BODY_LEG_ANGLE[i]);
        _footHome[i].x = BODY_LEG_ORIGINS[i][0] + STAND_RADIUS * cosf(a);
        _footHome[i].y = BODY_LEG_ORIGINS[i][1] + STAND_RADIUS * sinf(a);
        _footHome[i].z = -_prof.standHeight;
    }
}

void HexaGait::begin() {
    computeHome();
    for (int i = 0; i < 6; i++) legTargets[i] = _footHome[i];
}

void HexaGait::setMoveVector(float vx, float vy, float vyaw) {
    _vx = vx; _vy = vy; _vyaw = vyaw;
    bool wantMove = (fabsf(vx) + fabsf(vy) + fabsf(vyaw)) > 0.001f;
    if (wantMove && !_running) { _running = true; _cycleStart = millis(); }
    else if (!wantMove)        { _running = false; }
}

void HexaGait::update() {
    computeHome();   // ikut perubahan standHeight (profil medan)

    if (!_running) {
        // Settle ke home (berbasis waktu via faktor tetap; cukup halus).
        for (int i = 0; i < 6; i++) {
            legTargets[i].x = lerpf(legTargets[i].x, _footHome[i].x, 0.2f);
            legTargets[i].y = lerpf(legTargets[i].y, _footHome[i].y, 0.2f);
            legTargets[i].z = lerpf(legTargets[i].z, _footHome[i].z, 0.2f);
        }
        return;
    }

    unsigned long el = millis() - _cycleStart;
    float phase = fmodf((float)el, _prof.cycleTime) / _prof.cycleTime;  // 0..1

    for (int leg = 0; leg < 6; leg++) {
        // Tripod: grup {0,2,4} fase 0, grup {1,3,5} geser 0.5.
        float legPhase = phase + ((leg % 2 == 0) ? 0.0f : 0.5f);
        if (legPhase >= 1.0f) legPhase -= 1.0f;

        // Vektor langkah per kaki (mm) = translasi + rotasi(yaw).
        // Rotasi: v = omega x r  -> (-yaw*ry, yaw*rx).
        float rx = _footHome[leg].x, ry = _footHome[leg].y;
        float sx = (_vx + (-_vyaw * ry / 100.0f)) * _prof.stepLength;
        float sy = (_vy + ( _vyaw * rx / 100.0f)) * _prof.stepLength;

        float dx, dy, dz;
        if (legPhase < GAIT_DUTY) {
            // STANCE: dari +1/2 langkah ke -1/2 langkah (dorong badan maju), lurus.
            float s = legPhase / GAIT_DUTY;          // 0..1
            float k = 0.5f - s;                       // +0.5 -> -0.5
            dx = sx * k; dy = sy * k; dz = 0.0f;
        } else {
            // SWING: kembalikan -1/2 -> +1/2 sambil diangkat.
            float s = (legPhase - GAIT_DUTY) / (1.0f - GAIT_DUTY); // 0..1
            float k = -0.5f + s;                      // -0.5 -> +0.5
            dx = sx * k; dy = sy * k;
            dz = _prof.stepHeight * sinf(s * (float)M_PI);
        }

        legTargets[leg].x = _footHome[leg].x + dx;
        legTargets[leg].y = _footHome[leg].y + dy;
        legTargets[leg].z = _footHome[leg].z + dz;
    }
}
