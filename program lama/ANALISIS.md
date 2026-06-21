# Analisis Hexapod Lintas-Generasi & Roadmap Menang KRSRI

Tanggal: 2026-06-22
Repo dianalisis:
- `LEGACY2026/` — firmware Teensy 4.1, arsitektur 4-lapis (KONFIGURASI AKTIF / paling benar)
- `code_legacy_2024/` — Arduino, navigasi kompas + fuzzy wall-follow + Kalman (varian A)
- `Legacy/CODE_LEGACY_2024/` — Arduino, **firmware kontes 2024 paling lengkap** (varian B, R2CJUARA): ada lengan/capit + urutan misi penuh
- `hexapod_robot_legacy/` — generasi awal: Arduino + Raspberry Pi (vision OpenCV + lengan/arm)

---

## 1. Ringkasan Tiap Generasi

### LEGACY2026 (Teensy 4.1) — "yang paling benar"
Arsitektur bersih, modular, 4 lapis: `HexaServos` (2x PCA9685, non-blocking 50Hz) →
`HexaKinematics` (CMSIS-DSP matriks 4x4 + `LegIK` + `BodyKinematics`) → `HexaGait`
(tripod) → `Hexapod` (facade). Sensor: `IMUReader` (WT-series UART), `LidarArray`
(6x VL53L1X via mux TCA9548A), `FuzzyController` (penghindar halangan).

**Kekuatan:** struktur kode jauh lebih baik dari generasi lama, pakai FPU/SIMD,
servo non-blocking, IK ada clamp domain `acos` (anti-NaN).

**Masalah:** algoritmanya masih "warisan" — banyak yang turun dari 2024 tapi
DITURUNKAN KUALITASNYA (lihat bug di bawah). Robot ini secara fungsional BELUM bisa
belok, BELUM tahu arah hadap, dan BELUM punya deteksi korban.

### code_legacy_2024 — justru lebih pintar di navigasi
Punya yang HILANG di 2026:
- **Heading control via kompas** (`followUtara/Selatan/Barat/Timur`) — robot tahu arah.
- **Wall-following proporsional** (`followDindingKanan`) dengan hysteresis/selisih lidar.
- **Kalman filter** di pembacaan lidar.
- Fuzzy sudut yang sebenarnya (Sugeno-ish, 3 himpunan, defuzzifikasi weighted average).

Tapi: gaya kode `while()` blocking, `passData(n)` gerak diskrit (bukan gait kontinu),
arsitektur monolitik.

### hexapod_robot_legacy — vision & manipulasi
- `detection_n_measuring.py`: deteksi korban via HSV oranye + estimasi jarak (focal length).
- `program_mencari_objek.py`: lengan 6-DOF tracking objek (servo arm).
- Trajektori kaki versi MATLAB (`new_trayektori.m`).
Ini "mata + tangan" yang tidak pernah disatukan ke firmware Teensy.

---

## 2. BUG & KELEMAHAN KONKRET di LEGACY2026 (prioritas perbaikan)

### 🔴 KRITIS

1. **Belok (yaw) tidak diimplementasi sama sekali.**
   `HexaGait::update()` ada `// TODO: Tambahkan logika rotasi (Yaw) di sini`.
   `_moveYaw` disimpan tapi tidak dipakai. Padahal `FuzzyController` memanggil
   `robot.walk(y, 0, yaw)` — jadi SEMUA perintah belok dari fuzzy TIDAK ADA EFEK.
   Robot hanya bisa maju/mundur/geser, tidak bisa berputar. Untuk KRSRI (belok di
   sudut ruangan, align ke dinding) ini fatal.

2. **Trajektori kaki menyebabkan slip/oleng.**
   Z di-lift pakai `sin` saat fase swing, tapi X/Y pakai `cos(phase)` SEPANJANG
   siklus penuh — termasuk saat kaki menapak (stance). Artinya kaki yang sedang di
   tanah bergerak dengan kecepatan sinusoidal, bukan linear. Beberapa kaki stance
   bergerak beda kecepatan → kaki saling "berebut" → tergelincir, badan oleng,
   melenceng arah. Gait yang benar: stance = garis lurus kecepatan konstan, swing =
   angkat & kembalikan (cycloid/parabola).

3. **`LidarArray::update()` blocking ~100–140ms per loop.**
   6 sensor dibaca berurutan, tiap sensor `startRanging()` + busy-wait
   `while(!checkForDataReady()) delay(1)` (~20ms) + stop. Total ~120ms.
   Tapi `_servos->commit()` butuh dipanggil tiap 20ms (50Hz). Karena lidar
   memblok loop, servo cuma ter-update ~7–8x/detik → gerak patah-patah & gait rusak.
   Solusi: continuous ranging, baca 1 sensor per loop (round-robin), atau timer/DMA.

4. **Heading (yaw IMU) tidak dipakai.**
   `imu.getYaw()` ada tapi MainProgram cuma pakai roll/pitch buat stabilisasi.
   Tidak ada kontrol arah hadap. Navigasi KRSRI butuh tahu robot menghadap mana
   (grid ruangan). Kemampuan kompas dari 2024 hilang total.

5. **Tidak ada deteksi korban / mesin-status misi.**
   2026 hanya fuzzy "wander + hindar halangan". Tidak ada: cari korban, sinyal
   menemukan, hitung ruangan, kembali pulang. KRSRI butuh state machine misi.

### 🟠 PENTING

6. **Stabilisasi badan tanpa deadband & tanpa clamp.**
   `bodyRotation.x = -imu.getRoll()` langsung tanpa batas. Noise IMU → jitter terus;
   miring besar (di rubble/tangga) → sudut ekstrem → IK mentok/NaN → servo nyentak.
   Tambah deadband + low-pass + clamp (mis. ±15°).

7. **Fuzzy 2026 lebih lemah dari fuzzy 2024.**
   2026 cuma "depan dekat → mundur, samping dekat → belok". Tidak ada
   wall-following proporsional (jaga jarak konstan ke dinding) yang justru sudah
   ada & matang di `prog.h`/`controlSystem.h` 2024. Regresi.

8. **Kalibrasi servo terlalu sederhana.**
   `angleToPulse` = `map(angle,0,180,500,2500)` + offset + invert. Asumsi semua
   servo identik & linear 0–180° di 500–2500µs. RDS3235 nyata bervariasi & sering
   ~270°. Perlu per-servo gain/min/max (calibration table), bukan satu rumus global.
   (Ini "knob kalibrasi" yang wajib ada untuk hardware nyata.)

9. **`LegIK` warisan hack belum diverifikasi.**
   `if (x<0 && y<0) coxaAngle += 360;` dan `if (L <= FEMUR+TIBIA) tibiaAngle -= 90;`.
   Kondisi kedua hampir SELALU benar (kaki tak bisa melar > femur+tibia), jadi
   efektif konstanta `-90` yang disamarkan sebagai kondisi. Bersihkan & verifikasi
   frame sudut tibia terhadap nol servo asli.

10. **Settling gait frame-rate dependent.**
    Saat berhenti, `legTargets *= 0.95` per panggilan — bergantung kecepatan loop,
    bukan waktu. Pakai interpolasi berbasis `millis()`.

### 🟡 MINOR
- `new` di constructor tanpa `delete` (ok untuk single-instance, tapi bisa jadi member biasa).
- Sensor lidar index 1 tak terpakai di fuzzy.
- `MainProgram.ino` tidak cek return `lidar.begin()`.

---

## 3. APA YANG HARUS DILAKUKAN UNTUK MENANG KRSRI

KRSRI = navigasi ruangan + tangga + lantai pecah (rubble) + temukan korban + kembali.
Hexapod menang kalau ANDAL melewati medan & deteksi korban konsisten — bukan yang
paling canggih di atas kertas.

### Prioritas A — Bikin robot benar-benar bisa jalan & belok (1–2 minggu)
1. **Implementasi yaw di gait** (rotasi tiap kaki di sekitar pusat badan saat swing).
2. **Perbaiki trajektori**: pisah fase stance (linear) & swing (angkat-kembali).
   Pakai trajektori dari `new_trayektori.m` (2 generasi lalu) sebagai referensi.
3. **Lidar non-blocking** (round-robin 1 sensor/loop atau continuous mode) supaya
   servo tetap 50Hz.

### Prioritas B — Kembalikan "kepintaran" 2024 ke arsitektur 2026 (1–2 minggu)
4. **Heading hold pakai IMU yaw** (PID sederhana ke arah target) — port logika
   `followUtara/...` jadi modul `HeadingController` yang manggil `robot.walk(...,yaw)`.
5. **Wall-following proporsional** — port `followDindingKanan` jadi modul, jaga jarak
   konstan ke dinding kanan/kiri (error = setpoint − lidar_samping → yaw).
6. **Kalman/low-pass** pada lidar (sudah ada di 2024, tinggal pindah).

### Prioritas C — Medan KRSRI (rubble & tangga) (2–3 minggu)
7. **Adaptasi medan**: deteksi kontak tanah per kaki. Termurah: arus servo (sense)
   atau micro-switch di ujung tibia → tahu kaki sudah menapak → "ground probing"
   gait (turunkan kaki sampai kontak, baru lanjut). Ini pembeda juara di rubble.
8. **Mode tangga**: tinggikan `GAIT_STEP_HEIGHT` + perpanjang reach + pakai pitch IMU
   untuk pre-tilt badan. Buat profil gait per-medan (rata / rubble / tangga).
9. **Clamp + deadband stabilisasi** (lihat bug #6).

### Prioritas D — Misi & korban (paralel)
10. **Mesin-status misi** (state machine): SEARCH_ROOM → DETECT_VICTIM → SIGNAL →
    NEXT_ROOM → RETURN_HOME. Hitung ruangan via dinding + heading.
11. **Integrasi deteksi korban**: jalankan vision (kamera/termal) di Raspberry Pi
    (kode `detection_n_measuring.py` sudah ada), kirim hasil ke Teensy via Serial/I2C.
    Untuk KRSRI korban biasanya boneka/heat-source → kamera HSV + sensor suhu (mis.
    AMG8833/MLX90640) lebih andal daripada HSV warna saja.
12. **Sinyal menemukan korban** sesuai aturan lomba (LED/buzzer/lapor).

### Prioritas E — Daya tahan (jangan diremehkan; README 2024: "servo rusak saatnya pulang")
13. **Proteksi servo**: batasi arus/torsi, hindari perintah sudut mustahil (IK NaN),
    soft-start. Banyak tim kalah karena servo jebol di tengah lomba.
14. **Brownout/voltase**: monitor tegangan baterai; turunkan beban gait kalau drop.

---

## 4. Rekomendasi Arsitektur (gabungkan yang terbaik)

```
Teensy 4.1 (real-time, 50Hz):
  HexaServos → HexaKinematics → HexaGait(+yaw, multi-profil medan)
  IMUReader(roll/pitch stabil + yaw heading)
  LidarArray(non-blocking + Kalman)
  HeadingController (port dari 2024)
  WallFollow / FuzzyNav (port + perbaiki dari 2024)
  TerrainAdaptation (kontak kaki)
  MissionFSM (state machine)
        ↑ Serial/I2C ↓
Raspberry Pi (vision, tidak real-time):
  deteksi korban (kamera HSV/termal) + estimasi jarak → kirim koordinat ke Teensy
```

**Inti pesan:** kode 2026 = "kerangka bagus, isi algoritma masih kosong/regresi".
Kemampuan navigasi cerdas justru ada di 2024, dan mata/tangan ada di generasi awal.
Menang KRSRI = satukan ketiganya ke arsitektur 2026, dengan fokus utama: **belok +
trajektori benar + lidar non-blocking + heading/wall-follow + adaptasi medan +
deteksi korban + state machine misi.**

## 5. Urutan kerja yang disarankan (paling berdampak dulu)
1. Yaw gait (#1) — tanpa ini robot tak berguna.
2. Lidar non-blocking (#3) — tanpa ini gait selalu patah.
3. Trajektori stance/swing (#2).
4. Heading hold + wall-follow (#4,#5).
5. Adaptasi medan kontak kaki (#7) — pembeda juara.
6. State machine + deteksi korban (#10,#11).
7. Proteksi servo & daya (#13,#14).

---

## 6. TAMBAHAN: `Legacy/CODE_LEGACY_2024` (R2CJUARA) — firmware kontes paling lengkap

Repo ini adalah versi 2024 yang BENAR-BENAR dipakai lomba. Ia mengungkap struktur
misi KRSRI sebenarnya dan beberapa kemampuan KRUSIAL yang HILANG total di LEGACY2026.

### 6.1 Penemuan besar

**A. Robot punya LENGAN/CAPIT (gripper arm) — LEGACY2026 tidak punya sama sekali.**
`komponen.h` penuh fungsi lengan: `servo_ambil_korban_*`, `servo_taruh_korban`,
`servo_pindah_korban`, `servo_park_home`, `servo_naik_tangga`. Lengan = servo di
`kit0`/`kit1` channel 12,13,14 (body/tengah/capit). Inti tugas KRSRI = **mengambil &
memindahkan korban** dengan lengan ini. **LEGACY2026 `config.h` hanya memetakan 18
servo kaki — tidak ada channel lengan, tidak ada modul Arm.** Artinya robot 2026
saat ini secara fisik TIDAK BISA menyelesaikan tugas utama lomba. Ini gap nomor 1.

**B. Urutan misi penuh terungkap** (dari `loop()` + `kiri.h`):
```
menujuKorban1 → ambilKorban1 → ikutBarat → menujuSz1(safe zone) → ikutSelatan →
menujuKorban2 → menujuSz2 → menujuLantai2 → ambilKorban2 → menujuSz3 →
menujuTangga → naikTangga
```
Jadi arena: 2 korban, 3 safe zone, ada lantai 2 & tangga. Ini blueprint state
machine misi yang harus diport ke 2026 (lihat #10).

**C. Sudah ada multi-profil gait untuk medan** (yang juga hilang di 2026):
- `naikTangga()`: `Yoff-=20; lebarLangkah+=20; tinggiLangkah+=20;` — mode tangga.
- `ambilKorban2()`: `Zoff-=15; tinggiLangkah+=20;` lalu dikembalikan — menunduk/menjinjit.
- `setKaki()`: saat `Yoff==-20` (mode tangga) set `leg[i].putar`/`bukaan` per-kaki —
  **bukti rotasi/yaw per-kaki SUDAH ADA di 2024**, justru di 2026 jadi `// TODO`.

**D. Integrasi kamera via Serial** (`komponen.h::cekKamera`): baca char `l`/`r`/`c`
dari Serial (dikirim Raspberry Pi vision) → `passData(2/4/1)` untuk align kiri/kanan/
maju ke korban. Jadi loop vision→navigasi sudah pernah jalan lewat serial. Ada juga
sisa kode sensor IR (`cekIr`) untuk deteksi korban/lantai (di-comment).

**E. `bodyKinematics(roll,pitch,yaw)` + `applyBodyRotation`** sudah ada di 2024 —
konsep stabilisasi badan 2026 bukan barang baru, hanya dirapikan pakai CMSIS.

### 6.2 Kelemahan nyata firmware kontes ini (alasan "kadang error")

1. **Navigasi 100% scripted / dead-reckoning.** Pola: `while(lid0<=20){ ikutUtara();
   followDindingKanan(13,20); for(18) passData(1); }`. Jumlah langkah & ambang lidar
   di-hardcode. Tidak ada recovery loop tertutup: sekali drift/selip/korban geser,
   SELURUH rantai misi meleset (cocok dgn README lama: "masih kadang error"). **Ini
   akar kerapuhan yang harus dibenahi untuk menang konsisten.**
2. **`lid0..lid5` adalah MACRO** = tiap penyebutan memicu satu ranging blocking baru.
   `lid1 - lid2` = dua kali baca blocking; di kondisi `while` dibaca berkali-kali per
   iterasi → lambat & boros, dan dua sensor dibaca pada waktu berbeda (tidak sinkron).
3. **Repo ini TIDAK LENGKAP**: `CODE_LEGACY_2024.ino` `#include "ikutDinding.h"` dan
   `"kiri.h"`, tetapi **`ikutDinding.h` TIDAK ADA** di repo. Padahal di situlah
   `ikutUtara/Barat/Selatan/Timur` & `followDindingKanan/Belakang` didefinisikan
   (`algorithm.h` di varian B sudah dikosongkan, tinggal `kalmanFilter`). Jadi kode
   ini **tidak akan ter-compile apa adanya**. Implementasi wall-follow + kompas yang
   matang justru ada di varian A (`code_legacy_2024/prog.h` + `controlSystem.h` +
   `algorithm.h`). **Gabungkan: misi dari varian B + primitif navigasi dari varian A.**
4. **Penamaan fungsi** seperti `kontol()` — sisa kode bercanda, bersihkan sebelum lomba.
5. Gripper murni open-loop timing (`delay()`), tidak ada feedback cengkeraman → korban
   bisa lolos/jatuh. Pertimbangkan sensor (limit switch/IR/arus) di capit.

### 6.3 Revisi prioritas menang KRSRI (setelah temuan ini)

Gap terpenting BUKAN cuma gait — tapi: **2026 tidak bisa mengambil korban & tidak
punya urutan misi**. Robot 2026 yang sekarang tidak bisa menyelesaikan lomba sama
sekali. Prioritas direvisi:

- **P0 — Port LENGAN ke 2026.** Tambah `HexaArm` (channel servo lengan di config.h +
  fungsi `ambilKorban/taruhKorban/parkHome`). Tanpa ini, perbaikan lain percuma.
- **P0 — Mesin-status misi** mengikuti blueprint 6.1.B (state machine, bukan rantai
  `while` blocking). Tiap state punya syarat masuk/keluar + timeout + recovery.
- **P1 — Belok/yaw gait** (sudah ada konsep `leg.putar` di 2024 → port ke `HexaGait`).
- **P1 — Lidar non-blocking** + baca sekali per siklus, simpan ke array (jangan macro).
- **P1 — Wall-follow + heading dari varian A** (`prog.h`/`controlSystem.h`) sebagai
  modul closed-loop, gantikan dead-reckoning `passData()*N`.
- **P2 — Multi-profil gait medan** (rata/tangga/menunduk) — port `setKaki`/`naikTangga`.
- **P2 — Vision korban via Serial** (port `cekKamera`, upgrade ke termal bila perlu).
- **P3 — Feedback gripper + proteksi servo/baterai.**

### 6.4 Aset yang harus diselamatkan (jangan hilang lagi antar generasi)
| Kemampuan | Ada di | Status di LEGACY2026 |
|---|---|---|
| Lengan ambil korban | varian B `komponen.h` | ❌ HILANG (gap utama) |
| Urutan misi lengkap | varian B `loop()`+`kiri.h` | ❌ HILANG |
| Wall-follow proporsional | varian A `prog.h` | ❌ regresi (fuzzy lemah) |
| Heading kompas | varian A `algorithm.h` | ❌ HILANG (yaw IMU nganggur) |
| Multi-profil gait (tangga/nunduk) | varian B `kaki.h`/`kiri.h` | ❌ HILANG |
| Yaw/rotasi per-kaki | varian B `setKaki.putar` | ❌ `// TODO` |
| Vision korban (serial) | varian B `cekKamera` + raspi | ❌ belum terhubung |
| Kalman lidar | varian A & B | ❌ HILANG |
| IK + gait halus + CMSIS + stabilisasi | **LEGACY2026** | ✅ keunggulan baru |

**Kesimpulan akhir:** LEGACY2026 adalah lompatan kualitas di sisi *mekanik/gerak*
(IK, gait halus, FPU, stabilisasi) tapi *kemunduran fungsional besar* — kehilangan
seluruh kemampuan menyelesaikan misi (lengan, navigasi closed-loop, urutan misi,
multi-medan). Strategi menang = **pakai badan 2026 sebagai fondasi, lalu port balik
semua aset di tabel 6.4** (lengan & state machine lebih dulu).
