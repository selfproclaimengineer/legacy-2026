#include "HexaKinematics.h"

HexaKinematics::HexaKinematics(HexaServos* servos) {
    _servos = servos;
}

void HexaKinematics::begin() {
    // 1. Muat setup awal dari config.h
    for (int i = 0; i < 6; i++) {
        _legOrigins[i].x = BODY_LEG_ORIGINS[i][0];
        _legOrigins[i].y = BODY_LEG_ORIGINS[i][1];
        _legOrigins[i].z = BODY_LEG_ORIGINS[i][2];
    }
    
    // 2. Inisialisasi struct matriks CMSIS
    arm_mat_init_f32(&_T, 4, 4, _T_data);
    arm_mat_init_f32(&_RX, 4, 4, _RX_data);
    arm_mat_init_f32(&_RY, 4, 4, _RY_data);
    arm_mat_init_f32(&_RZ, 4, 4, _RZ_data);
    arm_mat_init_f32(&_TempMat, 4, 4, _TempMat_data);
    arm_mat_init_f32(&_BodyTransform, 4, 4, _BodyTransform_data);
    arm_mat_init_f32(&_vecIn, 4, 1, _vecIn_data);
    arm_mat_init_f32(&_vecOut, 4, 1, _vecOut_data);
    
    // 3. Set state awal ke Home
    setHomePose();
}

/**
 * Mengatur (reset) semua target public ke posisi Home.
 */
void HexaKinematics::setHomePose() {
    for (int i = 0; i < 6; i++) {
        legTargets[i] = {HOME_X, HOME_Y, HOME_Z};
    }
    bodyRotation = {0.0f, 0.0f, 0.0f};
    bodyTranslation = {0.0f, 0.0f, 0.0f};
}

/**
 * FUNGSI UPDATE (KALKULATOR MURNI)
 * Membaca state public dan menghitung IK/BK
 */
void HexaKinematics::update() {
    float coxaAngle, femurAngle, tibiaAngle;

    // --- 1. Bangun Matriks Transformasi Body ---
    BodyKinematics::translation(bodyTranslation.x, bodyTranslation.y, bodyTranslation.z, &_T);
    BodyKinematics::rotationX(bodyRotation.x, &_RX);
    BodyKinematics::rotationY(bodyRotation.y, &_RY);
    BodyKinematics::rotationZ(bodyRotation.z, &_RZ);

    arm_mat_mult_f32(&_RX, &_RY, &_TempMat);
    arm_mat_mult_f32(&_TempMat, &_RZ, &_BodyTransform);
    arm_mat_mult_f32(&_T, &_BodyTransform, &_BodyTransform);

    // --- 2. Terapkan Transformasi ke Setiap Kaki ---
    for (int legID = 0; legID < 6; legID++) {
        
        _vecIn_data[0] = legTargets[legID].x;
        _vecIn_data[1] = legTargets[legID].y;
        _vecIn_data[2] = legTargets[legID].z;
        _vecIn_data[3] = 1.0f;

        BodyKinematics::apply(&_BodyTransform, &_vecIn, &_vecOut);
        
        float x_body = _vecOut_data[0];
        float y_body = _vecOut_data[1];
        float z_body = _vecOut_data[2];

        float x_leg = x_body - _legOrigins[legID].x;
        float y_leg = y_body - _legOrigins[legID].y;
        float z_leg = z_body - _legOrigins[legID].z;
        
        LegIK::solve(x_leg, y_leg, z_leg,
                     coxaAngle, femurAngle, tibiaAngle);

        uint8_t coxaSlot = legID * 3 + 0;
        uint8_t femurSlot = legID * 3 + 1;
        uint8_t tibiaSlot = legID * 3 + 2;

        _servos->setPulse(coxaSlot, angleToPulse(coxaSlot, coxaAngle));
        _servos->setPulse(femurSlot, angleToPulse(femurSlot, femurAngle));
        _servos->setPulse(tibiaSlot, angleToPulse(tibiaSlot, tibiaAngle));
    }
}


// --- Fungsi Konversi Sudut ke Pulse ---
uint16_t HexaKinematics::angleToPulse(uint8_t servoID, float angle) {
    float calibratedAngle = SERVO_INVERT[servoID] ? -angle : angle;
    calibratedAngle += SERVO_ANGLE_OFFSET[servoID];
    float finalAngle = calibratedAngle + 90.0f;
    uint16_t pulse = map(finalAngle, 0, 180, SERVO_PULSE_MIN, SERVO_PULSE_MAX);
    return constrain(pulse, SERVO_PULSE_MIN, SERVO_PULSE_MAX);
}