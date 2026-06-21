# Dokumentasi Firmware Hexapod v2.0

Selamat datang di dokumentasi firmware hexapod. Proyek ini menggunakan arsitektur 4-Lapis yang modular dan *high-performance* yang dirancang untuk Teensy 4.1.

Seluruh kerumitan program disembunyikan di balik satu *class* (antarmuka) utama: `Hexapod`.

## Arsitektur Program

* **Lapis 1: `HexaServos`** (Otot)
    * Mengurus *hardware* (2x PCA9685).
    * Mengirim 18 sinyal *pulse* servo setiap 20ms (50Hz) secara *non-blocking*.
* **Lapis 2: `HexaKinematics`** (Tulang & Sendi)
    * Otak kalkulator (Matematika Inti).
    * Menggunakan CMSIS-DSP (SIMD) pada Teensy 4.1 untuk kalkulasi Matriks 4x4.
    * Menerjemahkan 6 target `(x,y,z)` kaki dan 1 pose tubuh menjadi 18 sudut servo (`LegIK`).
    * Mengonversi 18 sudut menjadi 18 *pulse* (kalibrasi).
* **Lapis 3: `HexaGait`** (Otak Kecil/Refleks)
    * Generator Pola Jalan (Gait Engine).
    * Menghasilkan 6 koordinat `(x,y,z)` yang mulus untuk pola jalan (Tripod Gait).
* **Lapis 4: `Hexapod`** (Fasad / Remote Control)
    * Ini adalah **satu-satunya** *class* yang perlu Anda gunakan di file `.ino` utama.
    * Memberikan perintah sederhana seperti `robot.walk()` atau `robot.goHome()`.
    * Mengurus logika interpolasi (gerak mulus) untuk pose tubuh.

---

## Cara Penggunaan (API `Hexapod`)

Ini adalah "tombol-tombol" pada remote control Anda.

### 1. Inisialisasi Dasar

Di file `.ino` utama Anda (`Hexapod_Refactor.ino`), Anda hanya perlu 3 baris kode untuk memulai:

```cpp
#include "Hexapod.h" // Hanya perlu 1 include ini

Hexapod robot; // 1. Buat objek robot

void setup() {
  robot.begin(); // 2. Inisialisasi semua layer
}

void loop() {
  robot.update(); // 3. Jalankan semua layer
}