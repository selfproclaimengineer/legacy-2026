#include "HexaArm.h"

HexaArm::HexaArm(HexaServos* servos, const uint8_t (*pinMap)[2]) {
    _servos = servos;
    _pin = pinMap;
    _moving = false;
    _t0 = _dur = 0;
    for (int i = 0; i < ARM_NUM_SERVOS; i++)
        _cur[i] = _start[i] = _goal[i] = ARM_POSE_PARK[i];
}

const float* HexaArm::poseData(ArmPose p) {
    switch (p) {
        case ARM_REACH: return ARM_POSE_REACH;
        case ARM_GRIP:  return ARM_POSE_GRIP;
        case ARM_LIFT:  return ARM_POSE_LIFT;
        case ARM_DROP:  return ARM_POSE_DROP;
        case ARM_PARK:
        default:        return ARM_POSE_PARK;
    }
}

void HexaArm::begin() {
    for (int i = 0; i < ARM_NUM_SERVOS; i++) {
        _cur[i] = ARM_POSE_PARK[i];
        writeAngle(i, _cur[i]);
    }
}

void HexaArm::goTo(ArmPose pose, uint32_t durationMs) {
    const float* g = poseData(pose);
    for (int i = 0; i < ARM_NUM_SERVOS; i++) { _start[i] = _cur[i]; _goal[i] = g[i]; }
    _t0 = millis();
    _dur = durationMs;
    _moving = true;
}

void HexaArm::update() {
    if (!_moving) return;
    uint32_t el = millis() - _t0;
    float t = (_dur == 0) ? 1.0f : (float)el / (float)_dur;
    if (t >= 1.0f) { t = 1.0f; _moving = false; }
    for (int i = 0; i < ARM_NUM_SERVOS; i++) {
        _cur[i] = lerpf(_start[i], _goal[i], t);
        writeAngle(i, _cur[i]);
    }
}

void HexaArm::writeAngle(uint8_t i, float deg) {
    deg = clampf(deg, 0.0f, 180.0f);
    uint16_t pulse = (uint16_t)(SERVO_PULSE_MIN +
                     (deg / 180.0f) * (SERVO_PULSE_MAX - SERVO_PULSE_MIN));
    _servos->writeRaw(_pin[i][0], _pin[i][1], pulse);
}
