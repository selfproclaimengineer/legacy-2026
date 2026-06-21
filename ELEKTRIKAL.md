# Kekurangan Elektrikal — yang perlu diperbaiki agar robot matang

Hexapod KRSRI 2026 (Teensy 4.1, 2× PCA9685, 24 servo, 6× VL53L1X, IMU Yahboom).
Daftar ini fokus **hardware/kelistrikan** — pelengkap `HEXAPOD_KRSRI_2026/README.md` (firmware).
Diurut per dampak ke lomba. **⚠️ Nilai bertanda `ISI:` wajib kamu lengkapi sesuai hardware nyata.**

> Konteks penting: berat robot **~5 kg**, ditumpu **3 kaki** saat tripod → torsi servo besar.
> Ada riwayat **"servo rusak"** dan tak ada **monitor tegangan** → robot bisa mati/reset di tengah
> lomba. Setelah R5 **tak ada retry otomatis** (Guidebook) → satu brownout = misi gagal.

---

## Tier 1 — Kritis (bisa menggagalkan lomba)

### 1.1 Tidak ada monitor tegangan baterai (risiko brownout)
**Masalah:** LiPo drop di bawah beban; bila tegangan jatuh, Teensy/servo reset → robot diam.
**Perbaikan:**
- Pembagi tegangan (2 resistor) dari V-batt → pin ADC Teensy (input maks **3.3 V**).
- Firmware: baca ADC → low-pass → histeresis; bila < ambang → `robot.stop()` + LED + cetak.
  (Hook firmware sudah direncanakan — lihat `CLAUDE.md` butir 2: `PIN_BATT`, `BATT_DIVIDER`, `BATT_MIN_V`.)

| Knob | Nilai | Catatan |
|---|---|---|
| Jumlah sel LiPo | `ISI:` (2S/3S/4S?) | menentukan V penuh/kosong |
| `PIN_BATT` (ADC) | `ISI:` | pin analog Teensy bebas |
| Rasio divider | `ISI:` | pastikan V-batt-maks × rasio ≤ 3.3 V |
| `BATT_MIN_V` | `ISI:` | mis. 3.5 V/sel di bawah beban |

### 1.2 Anggaran torsi & arus servo belum diverifikasi
**Masalah:** 5 kg di 3 kaki → tiap servo penahan beban besar; lonjakan arus saat semua servo
bergerak bisa **> 20 A puncak** (24 servo). BEC/regulator kurang kuat → tegangan ambruk → §1.1.
**Perbaikan:**
- Hitung torsi puncak per sendi (lengan momen × beban) vs rating servo (RDS3235 ≈ 35 kg·cm @ ISI: V).
- Suplai servo **terpisah & kuat** dari logic Teensy; UBEC/regulator sanggup arus puncak
  (ISI: rating kontinu/puncak A). Jangan ambil daya servo dari pin 5 V Teensy.
- Batasi percepatan gerak (slew gait sudah ada) agar arus inrush turun.

| Knob | Nilai | Catatan |
|---|---|---|
| Tegangan rail servo | `ISI:` | dalam rating servo (mis. 6.0–7.4 V) |
| Arus puncak terukur | `ISI: A` | ukur dgn clamp meter saat berdiri+bergerak |
| Rating BEC/UBEC | `ISI: A kontinu / A puncak` | harus > arus puncak terukur |

### 1.3 Distribusi daya & grounding
**Masalah:** kabel kurang besar / ground tidak common → drop tegangan, noise, reset acak.
**Perbaikan (checklist):**
- [ ] **Ground bersama** antara logic (Teensy) dan rail servo — wajib.
- [ ] Kabel daya servo AWG cukup (ISI: AWG) untuk arus puncak; jalur pendek.
- [ ] **Sekering** di jalur utama baterai (ISI: A) — proteksi korslet.
- [ ] **Saklar utama** + **proteksi polaritas terbalik** (dioda/MOSFET) di input baterai.
- [ ] **E-stop / kill switch** mudah dijangkau (putus daya servo).

---

## Tier 2 — Keandalan (jangan kehilangan poin yang sudah didapat)

### 2.1 Decoupling / kapasitor transien servo
Servo menyentak → spike tegangan ke PCA9685 & bus.
- [ ] Kapasitor elektrolit besar (mis. **1000 µF**, ISI sesuai) di rail servo tiap PCA9685.
- [ ] Ceramic **0.1 µF** dekat tiap VCC IC (PCA9685, mux, sensor).
- [ ] Kapasitor bulk di rail logic **3.3 V** agar Teensy tak ikut brownout.

### 2.2 Integritas bus I2C (1 jalur, banyak device)
Bus tunggal @400 kHz: 2× PCA9685 + mux + 6× VL53L1X + trace PCB.
- [x] Pull-up **2.2k–4.7k** (sudah dicatat di README §3). Verifikasi nilai terpasang.
- [ ] Cek level logic: VL53L1X **3.3 V**; pastikan tak ada device 5 V menarik SDA/SCL ke 5 V
      tanpa level-shift (Teensy 4.1 **tidak** 5 V-tolerant).
- [ ] Bila bus panjang/kapasitif & error → pertimbangkan buffer I2C (mis. PCA9517) atau turunkan clock.
- [ ] Tambah retry/timeout I2C di firmware bila device hang (lidar fail-safe sudah ada).

### 2.3 Watchdog & recovery daya
- [ ] **Watchdog** Teensy (`Watchdog_t4` / WDT_T4) — reset otomatis bila hang (CLAUDE.md butir 2).
- [ ] Pastikan setelah brownout/reset, boot ulang **aman** (servo tak menyentak ke pose ekstrem).

### 2.4 IMU & gangguan magnet (yaw)
- [ ] Kabel UART **921600** pendek & terpilin; jauh dari kabel arus servo (noise).
- [ ] Yaw (kompas) terganggu logam/medan motor → **jauhkan IMU** dari kabel daya & rangka besi.
      (Gate lonjakan yaw `IMU_MAX_YAW_JUMP` sudah ada, tapi sumber gangguan lebih baik dihilangkan.)

---

## Tier 3 — Pematangan (rapikan bila Tier 1–2 beres)

- [ ] **Konektor servo**: crimp kuat + strain relief; getaran kaki melonggarkan Dupont → kontak putus.
- [ ] **LED status/korban** (`PIN_LED_FOUND`) dengan resistor seri; cek aturan lomba soal indikator.
- [ ] **Pendinginan**: cek suhu PCA9685 & regulator setelah jalan lama; tambah heatsink/aliran udara bila panas.
- [ ] **Pelindung lidar**: pasang < 10 cm dari lantai (dinding arena 10 cm) + tutup dari debu koral/lumpur (R5/R7).
- [ ] **Manajemen kabel**: ikat rapi agar tak tersangkut rintangan / terinjak kaki.
- [ ] **EEPROM tuning** (sudah ada via GUI tuner) → tak perlu bongkar untuk recalibrate di venue.

---

## Checklist verifikasi sebelum lomba

- [ ] Ukur arus puncak (clamp meter) saat berdiri + berjalan; bandingkan rating BEC.
- [ ] Ukur tegangan rail servo saat semua servo bergerak (tidak boleh ambruk).
- [ ] Uji monitor baterai: turunkan tegangan → robot berhenti aman sebelum brownout.
- [ ] Uji E-stop: daya servo benar-benar putus.
- [ ] Goyang setiap konektor saat robot menahan beban → tak ada kedip/reset.
- [ ] Jalan 5 menit penuh (durasi lomba) tanpa reset/overheat.

> Isi semua `ISI:` di atas setelah mengukur hardware. Tanpa angka nyata, §1.1–§1.2 belum tuntas.
