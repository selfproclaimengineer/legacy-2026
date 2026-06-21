#ifndef Hexapod_h
#define Hexapod_h

#include "config.h"
#include "HexaServos.h"
#include "HexaKinematics.h"
#include "HexaGait.h"

class Hexapod {
public:
    Hexapod();
    void begin();
    void update();

    void walk(float y_speed = 0.0, float x_speed = 0.0, float yaw_speed = 0.0);
    void stop();
    void moveBodyRotation(float roll, float pitch, float yaw, 
                          uint32_t durationMs = DEFAULT_MOVE_DURATION);
    void moveBodyTranslation(float x, float y, float z, 
                             uint32_t durationMs = DEFAULT_MOVE_DURATION);
    void goHome(uint32_t durationMs = DEFAULT_MOVE_DURATION);

    HexaKinematics* kinematics;

private:
    HexaServos* _servos;
    HexaGait* _gait;

    uint32_t _poseMoveStartTime;
    uint32_t _poseMoveDuration;
    Point _poseStartBodyRot, _poseStartBodyTrans;
    Point _poseTargetBodyRot, _poseTargetBodyTrans;
    
    float lerp(float start, float target, float t);
};

#endif