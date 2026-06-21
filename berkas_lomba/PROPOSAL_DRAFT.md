# PROPOSAL — DRAF LENGKAP
## Lomba Robotik SAR — UNLIMITED 2026 (UNDIP)

> Draf siap diformat ke dokumen final (Word/PDF). **Maks 25 halaman**, PDF,
> nama file: `Lomba Robotik SAR_Nama PT_Nama Tim.pdf`.
> Bagian bertanda `[ISI]` diisi data tim. Gambar **tidak boleh** mencantumkan nama
> institusi. Target halaman tiap bab ditandai untuk menjaga total ≤ 25 hal.

---

## HALAMAN JUDUL  *(hal. 1)*

**PROPOSAL ROBOT SAR**
**UNLIMITED 2026**

Nama Tim : `[ISI — maks 15 karakter]`
Nama Robot : `[ISI]`
Perguruan Tinggi : `[ISI]`
Divisi : Lomba Robotik SAR

Logo PT `[ISI]`  ·  Tahun 2026

---

## LEMBAR PENGESAHAN  *(hal. 2)*

Yang bertanda tangan di bawah ini mengesahkan proposal tim **`[ISI]`** untuk mengikuti
Lomba Robotik SAR UNLIMITED 2026.

| | Nama | NIM/NIP | Tanda Tangan |
|---|---|---|---|
| Ketua Tim | `[ISI]` | `[ISI]` | |
| Dosen Pembimbing | `[ISI]` | `[ISI]` | |

Mengetahui, Pejabat Perguruan Tinggi (Wakil Rektor/Dekan Bid. Kemahasiswaan):
`[ISI nama, jabatan, tanda tangan, cap]`

*(Lampirkan scan surat pengantar resmi dari Pimpinan PT — disatukan dalam file PDF.)*

---

## DAFTAR ISI  *(hal. 3)*

1. Pendahuluan
2. Deskripsi & Spesifikasi Robot
3. Perangkat Lunak, Algoritma & Strategi
4. Penutup
- Lampiran A: Sketsa Desain (6 tampak)
- Lampiran B: Diagram Blok Sistem
- Lampiran C: Surat Pengantar PT

---

# BAB I — PENDAHULUAN  *(hal. 4–5)*

## 1.1 Latar Belakang
Lomba Robotik SAR UNLIMITED 2026 menantang robot otonom melewati 11 rintangan medan
pasca-bencana (jalan miring, jalan pecah, jalan berpuing, jalan berlumpur) sekaligus
menyelesaikan 5 misi penyelamatan korban. Diperlukan integrasi navigasi presisi,
kestabilan di medan tak rata, dan kecerdasan membedakan korban asli dari dummy. Tim kami
mengembangkan robot **hexapod (enam kaki)** karena konfigurasi berkaki memberi
keunggulan stabilitas statis (tripod gait selalu menapak tiga kaki) dan kemampuan
adaptasi pijakan pada permukaan tidak rata — sesuai karakter arena SAR.

## 1.2 Tujuan
1. Menyelesaikan lintasan R1–R11 dari Home hingga Finish secara otonom.
2. Menyelamatkan korban asli ke safety zone pasangannya dengan mekanisme cengkeram.
3. Memaksimalkan perolehan nilai (membawa korban menembus rintangan, R9, pembersihan
   SZ-3) dan meraih bonus waktu (finish + 5 misi < 300 detik).

## 1.3 Identitas Tim
- Nama Tim: `[ISI]` · Institusi: `[ISI]` · Alamat: `[ISI]`
- Ketua: `[ISI]` — email `[ISI]` — HP/WA `[ISI]`
- Anggota: `[ISI 1]`, `[ISI 2]`, `[ISI 3]`, `[ISI 4]`
- Dosen Pembimbing: `[ISI]`
- *(Lampirkan KTM seluruh anggota.)*

---

# BAB II — DESKRIPSI & SPESIFIKASI ROBOT  *(hal. 6–12)*

## 2.1 Informasi Umum
Robot merupakan robot berjalan jenis **hexapod** (enam kaki) bersimetri radial, dengan
**18 derajat kebebasan** (3-DOF per kaki: Coxa, Femur, Tibia) ditambah **dua lengan
penyelamat** bergripper. Robot beroperasi sepenuhnya otonom tanpa kendali jarak jauh.

| Parameter | Nilai |
|---|---|
| Jenis | Hexapod (6 kaki, radial simetris) |
| Dimensi (P×L×T) | 354.4 × 369.3 × 194.9 mm |
| Berat total | ± 5.0 kg |
| DOF kaki | 18 (3 × 6) |
| Lengan penyelamat | 2 (gripper mekanik, kanan & kiri) |
| Material | Akrilik (chassis), PLA (kaki & dome), bracket besi-U, karet ujung kaki |

## 2.2 Desain Mekanik & Material
Sasis menganut konfigurasi **double-deck**: dua lempeng akrilik heksagonal (deck atas &
bawah) mengapit servo pangkal kaki untuk meredam torsi. Material akrilik memberi
rigiditas menahan beban 5 kg tanpa melengkung. Sendi kaki (Coxa, Femur, Tibia) dan kubah
penutup (Top Dome) dicetak 3D dari **PLA** karena rasio kekuatan-terhadap-berat yang
baik; kubah poligonal melindungi elektronik. **Bracket besi-U** menjadi dudukan ekstra
penahan torsi tinggi, dan **ujung kaki karet berbentuk kerucut** menambah traksi agar
tidak tergelincir di medan licin/tak rata. Ruang tengah sasis menampung papan distribusi
daya, baterai, dan kontroler agar **pusat massa terpusat** demi kestabilan.

*(Sisipkan gambar penampang/eksplosi di sini atau rujuk Lampiran A.)*

## 2.3 Sistem Kendali & Penggerak
Arsitektur **dua-otak**: **Teensy 4.1** (ARM Cortex-M7 600 MHz + FPU) menangani tugas
real-time (gait, inverse kinematics, stabilisasi, sinyal servo 50 Hz non-blocking);
**Raspberry Pi 5** menangani pemrosesan citra. Keduanya berkomunikasi via UART 115200 bps.

Penggerak kaki: **18 servo digital metal-gear bertorsi tinggi** (kelas RDS3235, ~35 kg·cm),
dikendalikan **dua driver PWM PCA9685** (I2C, 0x40 & 0x41). PCA9685 #1 melayani kaki
kanan + lengan kanan; PCA9685 #2 melayani kaki kiri + lengan kiri. Kalibrasi pulsa
per-servo (offset, arah, trim) mengompensasi variasi mekanik.

Fungsi servo per kaki: **Coxa** (yaw — ayun maju/mundur), **Femur** (pitch — angkat/turun
kaki), **Tibia** (jangkauan ujung kaki ke tanah). **Dua lengan penyelamat** (kanan/kiri),
masing-masing bersendi dengan gripper, mencengkeram korban di bagian bawah lengannya;
dua lengan memungkinkan penanganan korban di kedua sisi tanpa memutar badan penuh.

## 2.4 Sistem Sensor
Tiga lapis sensor yang saling melengkapi:

1. **Jarak/navigasi — 6× VL53L1X (ToF laser, ≤4 m) via mux I2C TCA9548A (0x70).**
   Menghadap depan, kiri, kanan, belakang untuk wall-following, pengukuran lebar lorong,
   serta deteksi mulut ruang korban/safety zone. **Dipasang < 10 cm dari lantai** agar
   laser mengenai dinding arena yang tingginya hanya 10 cm. Pembacaan difilter
   low-pass/Kalman; pemindaian non-blocking (round-robin).

2. **Orientasi — IMU Yahboom 10-axis** (akselerometer 3, giroskop 3, magnetometer 3,
   barometer 1) dengan **algoritma Kalman bawaan** yang mengeluarkan sudut Euler & quaternion
   hingga 200 Hz via UART 921600 bps/USB/I2C, berbalut casing logam anti-interferensi.
   Roll & Pitch untuk **stabilisasi badan aktif** di jalan miring/pecah/berpuing/berlumpur.
   **Magnetometer memberi arah hadap absolut (kompas)** yang tidak drift seperti giroskop —
   menjaga orientasi pada rute berbelok. Barometer sebagai acuan perubahan ketinggian
   (tangga/miring). Magnetometer dikalibrasi terhadap gangguan medan magnet motor & logam.

3. **Penglihatan — Kamera + Raspberry Pi 5, model YOLOv8n (ONNX/onnxruntime).**
   Mendeteksi dan **mengklasifikasikan korban asli vs dummy** (pembeda: korban asli
   memiliki lengan). Menghitung posisi horizontal korban dalam bingkai dan kedekatannya,
   lalu mengirim ke Teensy via UART sebagai acuan pelurusan & pendekatan. Ketepatan ini
   menjadi inti perolehan nilai penyelamatan.

*(Diagram blok antarmuka sensor → Lampiran B.)*

## 2.5 Sistem Catu Daya
Baterai LiPo dengan **papan distribusi daya (PDB)** dan **regulator terpisah** antara
jalur logika (Teensy/Pi/sensor) dan jalur servo arus-tinggi, dengan ground disatukan.
**Soft-start** dan kapasitor penstabil mencegah brownout saat keenam kaki bergerak
serentak — aspek keandalan kritis pada robot berkaki berbeban.

---

# BAB III — PERANGKAT LUNAK, ALGORITMA & STRATEGI  *(hal. 13–20)*

## 3.1 Arsitektur Perangkat Lunak
Firmware Teensy berlapis: driver servo → kinematika (IK per kaki + transformasi badan) →
generator gait → fasad kendali → modul navigasi & mesin-misi. Raspberry Pi 5 menjalankan
pipeline vision terpisah dan menyuplai posisi korban. Pemisahan ini menjamin loop gerak
50 Hz tetap deterministik.

## 3.2 Navigasi (closed-loop)
Robot menelusuri lintasan dengan **wall-following proporsional** (menjaga jarak konstan
ke dinding via sensor samping) dipadu **heading-hold PID** terhadap kompas IMU. Pendekatan
loop-tertutup ini menggantikan dead-reckoning sehingga andal walau posisi korban diacak
juri dan lebar lorong hanya 45 cm. Sensor depan memicu perpindahan tahap misi.

## 3.3 Pola Jalan Adaptif (multi-profil gait)
Tripod gait dengan trajektori **fase tumpu lurus berkecepatan konstan** dan **fase ayun
terangkat** (anti-slip). Profil dipilih otomatis sesuai medan:
- **Datar** — lorong normal.
- **Tangga** — untuk M2 (anak tangga 2 cm × 3.6 cm): tinggi & panjang langkah diperbesar.
- **Merapat (narrow)** — untuk **R11 yang hanya menyisakan ~30 cm** akibat longsor: jejak
  kaki dirapatkan agar bentang < 30 cm.
- **Menunduk (crouch)** — menjangkau korban di ruang rendah.
Stabilisasi badan berbasis IMU menjaga kestabilan di seluruh jenis rintangan.

## 3.4 Deteksi & Klasifikasi Korban
YOLOv8n (ONNX) di Pi 5 membedakan **korban asli (berlengan)** dari **dummy (tanpa
lengan)**. Hanya korban asli yang ditindaklanjuti. Posisi korban (kiri/tengah/kanan +
kedekatan) dikirim ke Teensy untuk pelurusan badan dan pemilihan lengan penyelamat.

## 3.5 Mesin-Misi (Finite State Machine)
Logika misi mengikuti **urutan wajib** lintasan (mencoba K-1 sebelum R1, dst.). Tiap
tahap punya syarat masuk/keluar dan **batas-waktu + recovery** (mundur bila tersangkut).
Prosedur penyelamatan: deteksi korban asli → luruskan ke korban serong 45° → pilih lengan
kanan/kiri sesuai sisi → cengkeram bawah lengan → angkat keluar area → bawa ke safety
zone pasangannya.

## 3.6 Strategi Perolehan Nilai & Manajemen Waktu
Mengacu skema penilaian:
- **Membawa korban menembus rintangan = 150** (vs 100 kosong) → diutamakan.
- **R9 sambil membawa korban = 300** → target nilai tertinggi.
- **Pembersihan Safety Zone 3 (R7) = hingga 200** menggunakan kaki/badan (bukan boneka).
- **Menempatkan korban**: SZ-1..4 = 50, SZ-5 = 100.
- **Bonus**: finish + 5 misi < 300 s → `total × 300 / waktu`.
Manajemen waktu mengejar penyelesaian 5 misi + Finish < 300 detik, dengan **keandalan di
atas kecepatan** agar tidak gugur di medan sulit. Setiap rintangan dinilai 1×; hak
**retry** (1×) digunakan hanya bila benar-benar perlu.

## 3.7 Keandalan
Inverse kinematics dilengkapi pembatasan jangkauan (anti-NaN) agar servo tak menerima
perintah mustahil. Catu daya dengan soft-start & pemantauan tegangan mencegah brownout.
Ujung kaki karet dan pusat massa terpusat menjaga traksi serta kestabilan di rintangan.

---

# BAB IV — PENUTUP  *(hal. 21)*

Robot hexapod ini dirancang menyeimbangkan **kestabilan medan, navigasi presisi, dan
kecerdasan penyelamatan**. Dengan arsitektur dua-otak (Teensy + Raspberry Pi 5),
gait adaptif multi-profil, navigasi closed-loop berbasis lidar & kompas IMU Yahboom
10-axis, serta deteksi korban asli vs dummy berbasis YOLOv8n, tim kami menargetkan
penyelesaian seluruh misi Lomba Robotik SAR UNLIMITED 2026 secara optimal. Kami
berkomitmen menyempurnakan keandalan robot hingga hari pertandingan.

---

## LAMPIRAN A — SKETSA DESAIN  *(hal. 22–24)*
Enam tampak dengan dimensi (tanpa nama institusi pada gambar):
Isometrik · Depan · Tampak Kanan · Tampak Kiri · Tampak Belakang · Tampak Atas.
*(Sisipkan render/ gambar teknik.)*

## LAMPIRAN B — DIAGRAM BLOK SISTEM  *(hal. 25)*
```
                 +------------------- Raspberry Pi 5 --------------------+
   [Kamera] ---->| YOLOv8n (ONNX): deteksi & klasifikasi korban asli/dummy|
                 +---------------------------┬--------------------------+
                                             | UART 115200 (VIC state,kelas,posisi)
                                             v
 [6x VL53L1X]--mux TCA9548A--I2C--> +---------------- Teensy 4.1 ----------------+
 [Yahboom 10-axis IMU] ---UART----> | Navigasi closed-loop + Gait + IK + FSM misi |
 [Tombol Start] -------------------> +----------------┬-------------┬-------------+
                                             I2C(0x40)|     I2C(0x41)|
                                             v                       v
                                       PCA9685 #1              PCA9685 #2
                                   (kaki kanan + lengan    (kaki kiri + lengan
                                    penyelamat kanan)       penyelamat kiri)
                                             |                       |
                                        9 servo kaki kanan      9 servo kaki kiri
                                        + servo lengan kanan    + servo lengan kiri
```

## LAMPIRAN C — SURAT PENGANTAR PT
*(Sisipkan scan surat pengantar resmi dari Pimpinan Perguruan Tinggi.)*

---

> **Checklist submit (hari ini, 22 Juni 2026):**
> - [ ] Isi seluruh `[ISI]` + sketsa + diagram + scan surat pengantar
> - [ ] Total ≤ 25 halaman, ekspor PDF
> - [ ] Nama file `Lomba Robotik SAR_Nama PT_Nama Tim.pdf`
> - [ ] Bayar Rp 1.169.000 + upload bukti, KTM, bukti follow IG di unlimitedundip.com
