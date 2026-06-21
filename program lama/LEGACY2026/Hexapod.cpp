#include "Hexapod.h"

float Hexapod::lerp(float start, float target, float t) {
    return start + (target - start) * t;
}

Hexapod::Hexapod() {
    _servos = new HexaServos(SERVO_PIN_MAP);
    kinematics = new HexaKinematics(_servos); 
    _gait = new HexaGait(kinematics);
    
    _poseMoveStartTime = 0;
    _poseMoveDuration = 0;
}

void Hexapod::begin() {
    _servos->begin();
    kinematics->begin();
    _gait->begin();
    
    _poseStartBodyRot = kinematics->bodyRotation;
    _poseTargetBodyRot = kinematics->bodyRotation;
    _poseStartBodyTrans = kinematics->bodyTranslation;
    _poseTargetBodyTrans = kinematics->bodyTranslation;
}

void Hexapod::update() {
    if (_poseMoveDuration > 0) {
        uint32_t elapsedTime = millis() - _poseMoveStartTime;
        float t = 1.0f;
        if (elapsedTime < _poseMoveDuration) {
            t = (float)elapsedTime / (float)_poseMoveDuration;
        } else {
            _poseMoveDuration = 0;
        }
        kinematics->bodyRotation.x = lerp(_poseStartBodyRot.x, _poseTargetBodyRot.x, t);
        kinematics->bodyRotation.y = lerp(_poseStartBodyRot.y, _poseTargetBodyRot.y, t);
        kinematics->bodyRotation.z = lerp(_poseStartBodyRot.z, _poseTargetBodyRot.z, t);
        kinematics->bodyTranslation.x = lerp(_poseStartBodyTrans.x, _poseTargetBodyTrans.x, t);
        kinematics->bodyTranslation.y = lerp(_poseStartBodyTrans.y, _poseTargetBodyTrans.y, t);
        kinematics->bodyTranslation.z = lerp(_poseStartBodyTrans.z, _poseTargetBodyTrans.z, t);
    }
    
    _gait->update();
    kinematics->update();
    _servos->commit();
}

void Hexapod::walk(float y_speed, float x_speed, float yaw_speed) {
    _gait->setMoveVector(x_speed, y_speed, yaw_speed);
}

void Hexapod::stop() {
    _gait->setMoveVector(0.0, 0.0, 0.0);
}

void Hexapod::moveBodyRotation(float roll, float pitch, float yaw, uint32_t durationMs) {
    _poseStartBodyRot = kinematics->bodyRotation;
    _poseTargetBodyRot = {roll, pitch, yaw};
    _poseMoveStartTime = millis();
    _poseMoveDuration = durationMs;
}

void Hexapod::moveBodyTranslation(float x, float y, float z, uint32_t durationMs) {
    _poseStartBodyTrans = kinematics->bodyTranslation;
    _poseTargetBodyTrans = {x, y, z};
    _poseMoveStartTime = millis();
    _poseMoveDuration = durationMs;
}

void Hexapod::goHome(uint32_t durationMs) {
    stop();
    moveBodyRotation(0.0, 0.0, 0.0, durationMs);
    moveBodyTranslation(0.0, 0.0, 0.0, durationMs);
}