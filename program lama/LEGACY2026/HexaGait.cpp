#include "HexaGait.h"

HexaGait::HexaGait(HexaKinematics* kinematics) {
    _kinematics = kinematics;
    _running = false;
    _moveX = 0;
    _moveY = 0;
    _moveYaw = 0;
}

void HexaGait::begin() {
    for(int i=0; i<6; i++) {
        _kinematics->legTargets[i] = {HOME_X, HOME_Y, HOME_Z};
    }
}

void HexaGait::start() {
    if (_running) return; // Sudah berjalan
    _running = true;
    _cycleStartTime = millis();
}

void HexaGait::stop() {
    _running = false;
}

void HexaGait::setMoveVector(float x, float y, float yaw) {
    _moveX = x;
    _moveY = y;
    _moveYaw = yaw;

    if ( (x != 0 || y != 0 || yaw != 0) && !_running) {
        start();
    } 
    else if (x == 0 && y == 0 && yaw == 0) {
        stop();
    }
}

void HexaGait::update() {
    if (!_running) {
        // Jika berhenti, interpolasi kaki kembali ke Home
        for(int i=0; i<6; i++) {
            _kinematics->legTargets[i].x = _kinematics->legTargets[i].x * 0.95f;
            _kinematics->legTargets[i].y = HOME_Y + (_kinematics->legTargets[i].y - HOME_Y) * 0.95f;
            _kinematics->legTargets[i].z = HOME_Z + (_kinematics->legTargets[i].z - HOME_Z) * 0.95f;
        }
        return;
    }

    // --- GENERATOR TRIPOD GAIT ---
    unsigned long elapsedTime = millis() - _cycleStartTime;
    float phase = fmod(elapsedTime, GAIT_CYCLE_TIME) / GAIT_CYCLE_TIME;
    float phase_rad = phase * 2.0f * M_PI;

    float phase_tripod1 = phase_rad;
    float phase_tripod2 = fmod(phase_rad + M_PI, 2.0f * M_PI);

    for (int legID = 0; legID < 6; legID++) {
        float leg_phase = (legID % 2 == 0) ? phase_tripod1 : phase_tripod2;
        
        float x_pos, y_pos, z_pos;

        // Gerakan Vertikal (Z)
        if (leg_phase < M_PI) {
            z_pos = HOME_Z + GAIT_STEP_HEIGHT * sin(leg_phase);
        } else {
            z_pos = HOME_Z; // Kaki di tanah
        }
        
        // Gerakan Horizontal (X, Y)
        float step_factor = cos(leg_phase);
        
        // (Catatan: _moveX adalah geser, _moveY adalah maju/mundur)
        x_pos = -_moveX * GAIT_STEP_LENGTH * step_factor;
        y_pos = HOME_Y - _moveY * GAIT_STEP_LENGTH * step_factor; 
        
        // TODO: Tambahkan logika rotasi (Yaw) di sini
        
        _kinematics->legTargets[legID] = {x_pos, y_pos, z_pos};
    }
}