#include "HexaServos.h"

// Constructor: Menyimpan pointer ke peta pin
HexaServos::HexaServos(const uint8_t (*pin_map)[2]) {
    _pin_map = pin_map; // Simpan alamat dari pin map
    lastUpdateTime = 0;
}

// Inisialisasi: Siapkan driver & set servo ke posisi aman
void HexaServos::begin() {
    kit0.begin();
    kit1.begin();
    kit0.setPWMFreq(SERVO_FREQ);
    kit1.setPWMFreq(SERVO_FREQ);

    // Set semua servo ke 1500us (tengah) sebagai posisi aman
    for (int i = 0; i < NUM_SERVOS; i++) {
        targetPulses[i] = 1500;
    }
    
    // Langsung kirim perintah awal ini ke hardware
    // (mengabaikan timer untuk pertama kali)
    lastUpdateTime = millis();
    for (int i = 0; i < NUM_SERVOS; i++) {
        uint8_t kit_index = _pin_map[i][0];
        uint8_t pin_index = _pin_map[i][1];
        uint16_t pulse = targetPulses[i];

        if (kit_index == 0) {
            kit0.writeMicroseconds(pin_index, pulse);
        } else if (kit_index == 1) {
            kit1.writeMicroseconds(pin_index, pulse);
        }
    }
}

// Hanya menyimpan target baru di memori (array)
void HexaServos::setPulse(uint8_t servoID, uint16_t pulse) {
    if (servoID >= NUM_SERVOS) return; // Proteksi
    targetPulses[servoID] = pulse;
}

// Fungsi utama non-blocking
void HexaServos::commit() {
    // Cek apakah sudah 20ms berlalu
    if (millis() - lastUpdateTime < updateInterval) {
        return; // Belum waktunya, keluar
    }
    
    // Reset timer
    lastUpdateTime = millis();

    // Waktunya update! Kirim SEMUA 18 pulse
    for (int i = 0; i < NUM_SERVOS; i++) {
        
        // 1. Baca Peta
        uint8_t kit_index = _pin_map[i][0]; // Kit 0 atau 1?
        uint8_t pin_index = _pin_map[i][1]; // Pin 0-15?
        
        // 2. Ambil Nilai Pulse
        uint16_t pulse = targetPulses[i];

        // 3. Kirim Perintah
        if (kit_index == 0) {
            kit0.writeMicroseconds(pin_index, pulse);
        } else if (kit_index == 1) {
            kit1.writeMicroseconds(pin_index, pulse);
        }
    }
}