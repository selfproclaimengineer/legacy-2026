# Analisis Berkas Lomba — Robot SAR UNLIMITED 2026 (UNDIP)

Sumber: `Guidebook SAR.pdf` (37 hal) + `Borang Unlimited Legacy.docx`.
Tanggal analisis: 2026-06-22.

---

## ⚠️ 0. MENDESAK — DEADLINE PENDAFTARAN/PROPOSAL

**Open Registration: 25 Mei – 22 Juni 2026** (Guidebook hal. 7).
**Hari ini 22 Juni 2026 = HARI TERAKHIR.** Pastikan SEGERA:
- Proposal PDF + Borang terkirim via `unlimitedundip.com`.
- Bayar registrasi Rp 1.169.000 + upload bukti.
- Lampirkan KTM + surat pengantar pimpinan PT + bukti follow IG.
- Nama file: `Lomba Robotik SAR_Nama PT_Nama Tim.pdf` (maks 25 halaman).
- Timeline lanjut: Technical Meeting 17–18 Juli 2026; Pertandingan 18–19 Sept 2026.

---

## 1. CEK KEPATUHAN ROBOT (Borang vs Aturan)

Data robot dari Borang: hexapod 6 kaki, **354.4 (P) × 369.3 (L) × 194.9 (T) mm**,
**~5.0 kg**, 18 DOF (3/ kaki), akrilik + PLA, ujung kaki karet kerucut.

| Aturan (hal.) | Status | Catatan |
|---|---|---|
| Kaki ≥ 2, tiap kaki aktuator independen (19) | ✅ | 6 kaki, 18 servo independen |
| Hanya kaki menyentuh lantai, tanpa caster/penopang (19) | ✅ | pastikan Top Dome & bracket tak menyeret di rintangan |
| 1 tombol start/run, ditekan 1×; sound activation dilarang (20) | ✅ | firmware sudah 1 tombol |
| Gripper mekanik, cengkeram **hanya di bawah lengan korban** (15) | ⚠️ | desain capit harus menjepit bagian bawah lengan, bukan badan |
| Tanpa remote / intervensi (36) | ✅ | otonom penuh |
| **Lebar robot vs R11 (jalan bisa dilalui 30 cm) (29)** | ❌ **GAGAL** | Lebar 36.9 cm **tidak muat** di R11 (30 cm). Lihat §4. |
| Lebar robot vs lorong 45 cm (20) | ⚠️ | 36.9 cm → sisa ~4 cm/sisi. Wall-follow harus presisi |
| Berat 5 kg ditumpu kaki (servo) | ⚠️ | tripod = 3 kaki tahan 5 kg → torsi besar; risiko servo jebol (sejarah: "servo rusak"). Verifikasi anggaran torsi |
| Tinggi badan 19.5 cm vs dinding 10 cm | ⚠️ | badan lolos, tapi **lidar harus dipasang < 10 cm** agar "melihat" dinding |
| Tidak mengeluarkan api/air, tak merusak arena (36) | ✅ | aman |

**Kesimpulan kepatuhan:** Satu **blocker keras (lebar di R11)** + beberapa risiko.
Harus diatasi sebelum pertandingan (bukan soal proposal, tapi soal lolos lintasan).

---

## 2. STRUKTUR ARENA & MISI (ringkas, dengan dimensi)

Arena **360×240 cm**, lorong **45 cm**, dinding **tinggi 10 cm / tebal 2 cm**.
Permukaan dicat aquaproof (tidak licin). **11 rintangan (R1–R11)** + **5 korban (K1–K5)**
+ **5 safety zone (SZ1–SZ5)**. Waktu **300 detik**.

**Jenis rintangan (4):**
- **Jalan miring**: M1 (80×20 cm), **M2 = TANGGA** (90×50 cm, anak tangga tinggi 2 cm lebar 3.6 cm), M3 (50×5 cm).
- **Jalan pecah** (R6 45×55 cm): tiang diameter 3–4 cm, bawah tumpukan circle 2 cm.
- **Jalan berpuing**: batu koral putih **3–5 cm**.
- **Jalan berlumpur**: kelereng **15–17 mm** (R5 45×45 cm, 2 lapis).

**Korban & Safety Zone:**
- **K-1**: ruang 40×15 cm; 2 dummy + 1 asli; asli **serong 45°**; posisi diundi.
- **SZ-1**: R4 berpuing 45×60 cm, SZ 20×20 cm.
- **K-2**: 15×28 cm; 1 dummy + 1 asli serong 45°. **R5 berlumpur**. SZ-2 tinggi 4 cm, 20×20.
- **R6 jalan pecah** 45×55, bersebelahan K-3 & K-4.
- **K-3 & K-4**: ditimpa 2 papan 14×17×2 cm (ada coakan); korban serong 45°.
- **SZ-3**: harus **dibersihkan dari koral (= R7)** pakai tubuh robot (bukan boneka). 20×20.
- **SZ-4**: 20×20, tinggi 4 cm.
- **R10**: lebar 45 cm, koral + kelereng, miring 50.2 cm. **R11**: jalan **30 cm** (longsor kiri-kanan).
- **SZ-5**: bidang miring, ada **lubang 12×7 cm** untuk taruh boneka; belakang tinggi 7 cm.

**Korban:** oranye, **berat min 33 g**, model `.stl` panitia
(`https://kontesrobotindonesia.id/data/2021/korban.stl`).
**Beda asli vs dummy: korban ASLI punya LENGAN, dummy TANPA lengan.** ← kunci deteksi.

**Urutan WAJIB:** harus coba angkat K-1 **sebelum** melewati R1, dst. Mengambil korban
setelah melewati batas rintangannya = **tidak sah** (hal. 21).

---

## 3. PENILAIAN & STRATEGI MAKSIMAL POIN (hal. 34–35)

| Aksi | Poin |
|---|---|
| Keluar Home | 50 |
| Angkat korban keluar area korban | 50 |
| Lewati rintangan (R1–R11 kecuali R7,R9) **tanpa** korban | 100 |
| Lewati rintangan **dengan** korban | **150** |
| Lewati **R9** | 150 (tanpa) / **300 (dengan korban)** |
| Taruh korban di SZ-1..4 | 50 |
| Taruh korban di **SZ-5** | 100 |
| Bersihkan SZ-3 dari batu (R7) | 100 (area korban) / **200 (seluruh area)** |
| **Bonus finish + 5 misi < 300 s** | `total × 300 / waktu` |

**Implikasi strategi:**
1. **Membawa korban menembus rintangan bernilai +50% (100→150), dan R9 +100% (150→300).**
   → Strategi optimal: **angkat korban lalu bawa menembus rintangan** menuju SZ-nya,
   bukan sekadar lewat kosong. Tapi imbangi risiko jatuh saat membawa beban serong 45°.
2. **R9 adalah jackpot** (300 dengan korban). Pastikan robot mampu R9 sambil memegang korban.
3. **SZ-3 (R7) = 200 poin** kalau bersih seluruhnya — pembersihan pakai kaki/badan/lengan,
   **bukan** boneka. Sediakan rutin "menyapu" terpisah.
4. **Hanya 1× nilai per rintangan** (nilai terbaik). Jangan buang waktu mengulang.
5. **Deteksi asli vs dummy menentukan segalanya** — salah angkat dummy = buang waktu &
   tak dapat poin penyelamatan. Prioritas keandalan vision (lihat §4).
6. **Bonus waktu** menggandakan strategi: selesaikan 5 misi + finish < 300 s →
   makin cepat, makin besar pengali. Tapi jangan korbankan keandalan demi kecepatan.
7. **Retry 1×** hanya setelah keluar Home & diam >10 s; waktu tetap jalan. Pakai bijak.
   Hitung mundur 10 "error" **tidak berlaku mulai R5** → setelah R5 tak ada retry otomatis.

---

## 4. IMPLIKASI KE DESAIN ROBOT & FIRMWARE (yang harus diperbaiki)

Hubungkan ke kode `../HEXAPOD_KRSRI_2026/` & `../RASPI_VISION_KRSRI/`:

1. **🔴 Lebar robot di R11 (30 cm).** Robot 36.9 cm tidak muat.
   - Solusi: **profil gait "NARROW"** — kecilkan `STAND_RADIUS` (rapatkan kaki) saat R11
     agar bentang kaki < 30 cm. Tambah `profileNarrow()` di `Hexapod` + state khusus R11.
   - Verifikasi mekanik: bentang kaki minimum (kaki ditekuk masuk) harus ≤ 28–29 cm.
2. **🔴 Deteksi asli vs dummy (vision).** Update `RASPI_VISION_KRSRI`: latih YOLOv8n
   **2 kelas** (`korban_asli`, `dummy`) — pembeda = **ada/tidak lengan**. Protokol UART
   tambah field kelas: `VIC <state> <class> <x_norm> <area> <conf>`. Robot hanya angkat
   `class=asli`.
3. **Lidar < 10 cm** dari lantai (dinding cuma 10 cm). Sesuaikan pemasangan & verifikasi
   VL53L1X membaca dinding pada tinggi itu. (Catatan ke `config.h`/wiring.)
4. **Gripper menjepit bawah lengan korban serong 45°.** Lengan/`HexaArm` harus bisa
   mendekati pada sudut 45°. Pertimbangkan derajat kebebasan rotasi capit / pendekatan
   badan menyudut. Tambah pose `ARM_GRIP_45`.
5. **Profil tangga M2** (anak tangga 2 cm, lebar 3.6 cm): `profileStairs()` sudah ada —
   tune `stepHeight`/`stepLength` agar pijakan pas 3.6 cm.
6. **Medan berlumpur (kelereng) & berpuing (koral):** butuh **adaptasi pijakan** (deteksi
   kontak kaki) — sudah jadi rekomendasi P-medan di `ANALISIS.md`. Ini krusial di R5/R10.
7. **Membawa korban menembus rintangan:** gait harus stabil saat CoG bergeser oleh beban
   korban + lengan terjulur. Uji stabilisasi IMU dengan beban.
8. **State machine misi** harus mengikuti **urutan wajib** (coba K-1 sebelum R1, dst.) —
   struktur FSM di `Mission.cpp` sudah sesuai polanya, tinggal petakan ke R1..R11/K1..K5.

---

## 5. CHECKLIST ISI PROPOSAL/BORANG (Lampiran A, hal. 10 & 36–37)

Wajib ada (≤ 25 halaman, PDF, gambar tanpa nama institusi):

- [ ] **Identitas tim**: ketua + anggota (5 mhs aktif) + 1 dosen pembimbing, institusi,
      alamat, email, no. HP ketua (untuk WA grup); + KTM + lembar pengesahan PT.
- [ ] **Informasi umum robot**: jenis & jumlah robot (1 hexapod).
- [ ] **Desain robot**: dimensi (354.4×369.3×194.9 mm), berat (~5 kg), struktur mekanik,
      bahan (akrilik + PLA + bracket besi U + karet ujung kaki) — **sudah ada draf di Borang**.
- [ ] **Sistem kendali & penggerak**: Teensy 4.1 + 2× PCA9685 + 18 servo (coxa/femur/tibia),
      double-deck chassis, PDB, baterai. (Borang baru sebut servo umum — **tambah detail
      mikrokontroler & driver**.)
- [ ] **Sistem sensor + interface**: 6× VL53L1X (via mux TCA9548A) untuk dinding/jarak,
      IMU WT (roll/pitch/yaw), **kamera + Raspberry Pi 5 (YOLOv8n→ONNX) untuk deteksi korban
      asli vs dummy**. (Borang **belum mencantumkan sensor sama sekali — WAJIB diisi**.)
- [ ] **Algoritma/strategi**: navigasi wall-follow + heading IMU closed-loop, FSM misi,
      multi-profil gait (datar/tangga/narrow/crouch), strategi bawa-korban-menembus-rintangan
      untuk poin 150/300, pembersihan SZ-3. (Borang **belum diisi — WAJIB**.)
- [ ] **Sketsa desain** 6 tampak (isometrik, depan, kanan, kiri, belakang, atas) dengan
      dimensi, **tanpa nama institusi pada gambar**.

**Yang masih KOSONG di Borang saat ini:** Sistem Sensor, Algoritma/Strategi, dan detail
Sistem Kendali (mikrokontroler/driver). Ini bagian bernilai tinggi untuk evaluasi —
**prioritaskan mengisinya sebelum submit hari ini.**

> Materi untuk mengisi 3 bagian itu sudah tersedia lengkap di
> `../HEXAPOD_KRSRI_2026/README.md` (arsitektur, sensor, algoritma) dan
> `../RASPI_VISION_KRSRI/README.md` (deteksi korban) — tinggal disarikan ke Borang.
