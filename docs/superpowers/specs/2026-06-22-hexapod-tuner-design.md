# Hexapod Tuner — Spec Desain

Tanggal: 2026-06-22
Repo: `HEXAPOD_KRSRI_2026/` (Teensy 4.1) + GUI laptop.

## Tujuan

Satu alat untuk **memasang servo selaras program** dan **menyetel knob kalibrasi di venue
tanpa recompile**. Hasil tuning disimpan di EEPROM Teensy; robot ingat sendiri saat hari-H
(laptop dicabut). GUI berupa satu file HTML (Web Serial) — nol instalasi.

Cakupan disetujui: servo kaki + servo lengan + knob gerak (gait, PID heading/wall, stabilisasi,
WALL_SETPOINT, HEAD_*), + pose lengan (`ARM_POSE_*`).

Non-tujuan (YAGNI): grafik telemetri realtime, multi-profil tersimpan, autentikasi,
IK lengan penuh. Bisa ditambah belakangan; tidak dibutuhkan untuk kalibrasi.

## Arsitektur

```
[hexapod_tuner.html] --Web Serial USB 115200--> [Teensy: SerialTuner] --> [Calib (RAM)] --> [EEPROM]
                                                          |                     ^
                                                          v                     |
                                              angleToPulse / Pid / gait  <------+  (baca runtime)
```

Boot = jalur normal **tak berubah**. Mode TUNE hanya aktif bila GUI mengirim `MODE TUNE`.

### Komponen baru
| File | Tanggung jawab | Bergantung pada |
|---|---|---|
| `Calib.h/.cpp` | Sumber tunggal nilai runtime + tabel default + load/save EEPROM + CRC16 | `<EEPROM.h>`, `config.h` |
| `SerialTuner.h/.cpp` | Parser baris ASCII di `Serial`; eksekusi perintah; jog servo; suspend misi | `Calib`, `HexaServos`, `Hexapod`, `HexaArm` |
| `hexapod_tuner.html` | GUI Web Serial: render slider dari DUMP, kirim perintah live | Chrome/Edge (Web Serial) |

### Komponen diubah (minimal, nama simbol dipertahankan)
| File | Perubahan |
|---|---|
| `config.h` | Array servo/lengan calib `const` → `extern` (definisi di `Calib.cpp`). Scalar knob `#define X 0.1f` → `#define X gParam[K_X]`. Geometri (`COXA/FEMUR/TIBIA`, `STAND_*`, pin map) **tetap const**. |
| `HEXAPOD_KRSRI_2026.ino` | `tuner.update()` dipanggil awal loop; bila mode TUNE → lewati `mission.update()`+gait, tetap `robot` commit servo (untuk jog/HOME). EEPROM di-load di `setup()` sebelum `robot.begin()`. Tanda stabilisasi `setStabilization(K_STAB_SIGN_ROLL*roll, K_STAB_SIGN_PITCH*pitch)` baca runtime (default −1). |
| `Mission.cpp` | Tiap `update()`: refresh `_headPid.kp/kd`,`_wallPid.kp/kd` dari runtime → PID live saat tuning. |

`Hexapod::angleToPulse()` **tak diubah** — sudah membaca `SERVO_OFFSET/INVERT/TRIM/PULSE_MIN/MAX`
yang kini runtime. Itu satu-satunya konsumen calib servo kaki (terverifikasi via grep).

## Calib: model data

### Registry parameter skalar
```c
enum ParamId {
  K_PULSE_MIN, K_PULSE_MAX,
  K_GAIT_STEP_HEIGHT, K_GAIT_STEP_LENGTH, K_GAIT_CYCLE_TIME, K_GAIT_DUTY,
  K_GAIT_SLEW_RATE, K_GAIT_PROFILE_TAU, K_GAIT_SETTLE_TAU,
  K_HEADING_KP, K_HEADING_KD, K_WALL_KP, K_WALL_KD, K_WALL_SETPOINT,
  K_STAB_SIGN_ROLL, K_STAB_SIGN_PITCH,
  K_HEAD_UTARA, K_HEAD_TIMUR, K_HEAD_SELATAN, K_HEAD_BARAT,
  N_PARAMS
};
struct ParamDef { const char* name; float def, lo, hi; };
extern const ParamDef PARAM_DEFS[N_PARAMS];   // di flash (PROGMEM-friendly)
extern float gParam[N_PARAMS];                 // nilai runtime
```
Tambah knob baru = 1 baris enum + 1 baris `PARAM_DEFS`. GUI render otomatis (lo/hi → rentang slider).

### Array servo (jog + per-sendi)
```c
#define NUM_TUNE_SERVOS 24            // 18 kaki + 3 lengan kanan + 3 lengan kiri
extern float   gOffset[NUM_SERVOS];  // hanya kaki (lengan pose-based)
extern int16_t gTrim[NUM_SERVOS];
extern uint8_t gInvert[NUM_SERVOS];
// Peta pin gabungan utk JOG <id>: id 0..17 = SERVO_PIN_MAP, 18..20 = ARM_R, 21..23 = ARM_L
const uint8_t TUNE_PIN_MAP[NUM_TUNE_SERVOS][2];   // dibangun dari peta yang ada
```

### Pose lengan tunable
`ARM_POSE_PARK/REACH/GRIP/LIFT/DROP` (5 × 3 = 15 float) → array runtime `gArmPose[5][3]`,
diekspos sebagai param bernama `arm.<pose>.<j>` (mis. `arm.grip.2`). `HexaArm::poseData()`
membaca `gArmPose` alih-alih const.

### EEPROM
Blob ber-versi, ditulis sekali di alamat 0:
```
magic 'H','X' | uint8 version | float gParam[N_PARAMS]
              | float gOffset[18] | int16 gTrim[18] | uint8 gInvert[18]
              | float gArmPose[5][3] | uint16 crc16
```
Boot `Calib::load()`: cek magic+version+CRC. Valid → pakai; tidak valid/kosong → isi dari
`PARAM_DEFS`+const default lalu `save()`. `save()` pakai `EEPROM.put` (Teensy: emulasi flash,
wear cukup). CRC16-CCITT atas seluruh blob kecuali field crc.

## Protokol serial (ASCII, `\n`-terminated, 115200)

PC → Teensy:
```
MODE TUNE | MODE RUN
DUMP
SET <name> <float>            ; param skalar atau arm.<pose>.<j>
SVO <id> <offsetDeg> <trimUs> <invert0/1>   ; kalibrasi 1 servo kaki
JOG <id> <pulseUs>            ; gerak mentah 1 servo (mode TUNE saja)
HOME                          ; perintah pose berdiri (lihat efek calib)
SAVE | LOAD | DEFAULTS
```
Teensy → PC:
```
OK
ERR <pesan>
; balasan DUMP:
P <name> <val> <lo> <hi>
SVO <id> <offset> <trim> <invert>
ARM <pose> <j> <val>
END
```
Aturan: perintah tak dikenal → `ERR unknown`. Nilai di luar [lo,hi] → di-clamp + `OK`.
`JOG`/`HOME` di luar mode TUNE → `ERR not in tune mode`. Parser: baca sampai `\n`, buang `\r`,
tokenisasi spasi, abaikan baris kosong. Buffer baris maks 64 char (lebih → `ERR overflow`).

## GUI `hexapod_tuner.html`

Satu file statis, tanpa build. Alur:
1. Tombol **Connect** → `navigator.serial.requestPort()` → buka 115200.
2. Kirim `MODE TUNE`, lalu `DUMP`. Parse balasan sampai `END`.
3. Render:
   - **Tabel 24 servo**: slider JOG (`SERVO_PULSE_MIN..MAX`), tombol "Netral 1500µs",
     dan untuk 18 kaki: input offset, trim, checkbox invert.
   - **Panel knob**: 1 slider per baris `P` (label = name, rentang = lo..hi, step halus).
   - **Panel pose lengan**: 5 pose × 3 angka.
4. Interaksi → kirim perintah live, **throttle ~50 ms** (≈20 Hz) per kontrol agar serial tak banjir.
5. Tombol global: **HOME**, **SAVE**, **LOAD**, **DEFAULTS**, **MODE RUN** (keluar tuning).
6. Area log menampilkan baris masuk (`OK`/`ERR`) untuk umpan balik.

Catatan: Web Serial hanya jalan di Chromium (Chrome/Edge) dan butuh origin aman —
file lokal `file://` diizinkan di Chrome desktop. Tulis di README.

## Keamanan lomba

- Default boot tak berubah → tak ada risiko mode tuning aktif tak sengaja saat lomba.
- Mode TUNE: `mission.update()` dan gait di-skip; servo tetap `commit()` (jog/HOME).
- Tombol START diabaikan selama mode TUNE.
- Keluar via `MODE RUN` atau reset daya.
- Watchdog (item handoff #2, terpisah) tetap di-feed di kedua mode.

## Pengujian (host, tanpa hardware)

`test/test_calib.cpp` (g++; fallback verifikasi Python seperti test IK):
1. **CRC round-trip**: pack blob → ubah 1 byte → CRC berubah; tak diubah → cocok.
2. **load() default**: blob kosong/magic salah → semua `gParam == PARAM_DEFS[i].def`.
3. **Tabel param waras**: nama unik, `lo <= def <= hi` untuk tiap entri.
4. **Clamp SET**: nilai di luar rentang → ter-clamp ke [lo,hi].

CRC + pack/unpack dipisah ke fungsi murni (tanpa `<EEPROM.h>`) supaya bisa dikompilasi di host.

## Dokumentasi (deliverable)

Bagian baru `README.md` §Kalibrasi-GUI: langkah pasang horn → set netral (JOG 1500) →
pasang horn lurus → set offset/trim per sendi → HOME verifikasi → tuning wall/PID jalan →
SAVE. Plus catatan browser Web Serial.

## Urutan implementasi (untuk plan)

1. `Calib.h/.cpp` + fungsi murni CRC/pack + test host. (pondasi, tak ganggu firmware lain)
2. Sambungkan `config.h` ke runtime (extern + macro redirect); kompil firmware tetap hijau.
3. `SerialTuner.h/.cpp` + integrasi `.ino` (mode, suspend misi) + refresh PID di `Mission`.
4. `hexapod_tuner.html`.
5. README §Kalibrasi-GUI.

Tiap langkah berdiri sendiri & dapat diverifikasi (host test / kompil) sebelum lanjut.
