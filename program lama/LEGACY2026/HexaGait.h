#ifndef HexaGait_h
#define HexaGait_h

#include "HexaKinematics.h" // Perlu akses ke Lapis 2
#include <math.h>          // Untuk sin()

class HexaGait {
public:
    HexaGait(HexaKinematics* kinematics);
    
    void begin();
    void update();

    void start();
    void stop();
    void setMoveVector(float x, float y, float yaw);

private:
    HexaKinematics* _kinematics;
    
    bool _running;
    unsigned long _cycleStartTime;
    
    float _moveX, _moveY, _moveYaw;
};

#endif