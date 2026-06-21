# Raspberry Pi 5 — Deteksi Korban KRSRI (YOLOv8n → ONNX)

Subsistem **penglihatan** hexapod KRSRI. Berjalan di **Raspberry Pi 5**, terpisah dari
firmware gerak di Teensy 4.1 (`../HEXAPOD_KRSRI_2026/`).

**Tugasnya satu**: temukan korban di frame kamera, hitung **di mana** posisinya
(kiri/tengah/kanan + perkiraan jarak), lalu kirim ke Teensy lewat **UART**. Teensy yang
memutuskan gerak (state `ALIGN_VICTIM` di FSM misi). Pembagian ini sengaja:
- **Pi 5** = berat sebelah ke komputasi (inferensi neural network, tidak real-time-kritis).
- **Teensy** = real-time 50Hz (gait, IK, servo) — tidak boleh diganggu beban vision.

```
[Kamera] -> [Pi 5: YOLOv8n ONNX] --UART "VIC ..."--> [Teensy: ALIGN_VICTIM] -> ambil korban
```

---

## 1. Kenapa YOLOv8n + ONNX (bukan ultralytics langsung / HSV warna)

- **YOLOv8n** = varian *nano*, paling ringan, cukup untuk 1–2 kelas (korban). Jauh lebih
  andal dari deteksi warna HSV generasi lama (`detection_n_measuring.py`) yang gampang
  tertipu pencahayaan arena.
- **ONNX + onnxruntime** = inferensi tanpa menyeret seluruh PyTorch/ultralytics ke Pi.
  Lebih ringan dipasang, start cepat, hemat RAM, dan onnxruntime memanfaatkan **NEON**
  (SIMD ARM) di Pi 5 → FPS lebih tinggi di CPU.
- Pi 5 tidak punya GPU CUDA. Inferensi jalan di **CPU** (4× Cortex-A76). Untuk realtime,
  pakai input kecil (lihat §7) atau akselerator opsional (Hailo-8L via PCIe HAT) bila ada.

---

## 2. Hardware

| Komponen | Catatan |
|---|---|
| Raspberry Pi 5 (4/8 GB) | otak vision |
| Kamera | **Pi Camera Module 3** (CSI, pakai `picamera2`) **atau** webcam USB (pakai `cv2.VideoCapture`) |
| Pendingin | Pi 5 panas saat inferensi terus-menerus — **wajib heatsink + fan** |
| UART ke Teensy | GPIO14 (TXD, pin 8) Pi → RX Teensy; GND ↔ GND. **3.3V, jangan 5V.** |
| Power | suplai Pi 5 terpisah & stabil (5V/5A). Jangan ambil dari rel servo. |

> Alternatif link: USB-Serial (Pi USB ↔ Teensy USB). Lebih gampang tapi pakai 1 port USB.
> Default dokumen ini: **UART GPIO** (`/dev/ttyAMA0` di Pi ↔ `Serial2`/`Serial3` di Teensy).

---

## 3. Stack perangkat lunak

- Raspberry Pi OS (64-bit, Bookworm).
- Python 3.11+
- `onnxruntime` (CPU build untuk ARM64)
- `opencv-python` (capture + preprocessing + NMS)
- `numpy`
- `pyserial` (kirim ke Teensy)
- `picamera2` (hanya jika pakai kamera CSI)

---

## 4. Menyiapkan model (latih → ekspor ONNX)

Lakukan di PC (bukan di Pi), lalu salin file `.onnx` ke Pi.

1. **Dataset**: foto korban KRSRI (boneka/silinder) dari berbagai sudut, jarak,
   pencahayaan arena. Label 1 kelas: `korban`. (Roboflow/labelImg.)
2. **Latih** YOLOv8n:
   ```bash
   yolo detect train model=yolov8n.pt data=korban.yaml imgsz=640 epochs=100
   ```
3. **Ekspor ke ONNX** (opset 12, input tetap):
   ```bash
   yolo export model=runs/detect/train/weights/best.pt format=onnx imgsz=640 opset=12 simplify=True
   ```
   Hasil: `best.onnx`. Input `1x3x640x640`, output `1x5x8400` (untuk 1 kelas:
   `[x,y,w,h,conf]` per anchor; untuk N kelas: `1x(4+N)x8400`).
4. Salin `best.onnx` ke Pi (mis. `~/vision/model/korban.onnx`).

> **Penting** — catat resolusi `imgsz` yang dipakai ekspor; preprocessing di Pi harus sama.
> Untuk FPS lebih tinggi di Pi, boleh ekspor ulang `imgsz=320` (lihat §7, ada trade-off akurasi).

---

## 5. Pipeline deteksi (di Pi)

Tiap frame:

1. **Capture** frame BGR dari kamera.
2. **Letterbox** ke `imgsz × imgsz` (jaga rasio, padding abu-abu) — JANGAN resize paksa,
   bisa menggeser posisi bbox.
3. **Preprocess**: BGR→RGB, `/255.0`, HWC→CHW, tambah batch → `float32 1x3xS xS`.
4. **Inferensi** `onnxruntime.InferenceSession.run(...)`.
5. **Postprocess**:
   - decode output → kotak `(x,y,w,h)` + skor,
   - buang skor < `CONF_THRESH`,
   - **NMS** (`cv2.dnn.NMSBoxes`) buang kotak tumpang tindih,
   - kembalikan koordinat ke skala frame asli (lepas letterbox).
6. **Pilih korban**: ambil bbox skor tertinggi (atau terbesar — korban terdekat).
7. **Hitung posisi** (lihat §6).
8. **Kirim** ke Teensy (lihat §7-protokol).

---

## 6. Lokalisasi korban (output yang dikirim)

Dari bbox terpilih hitung:

- **`x_norm`** = posisi horizontal pusat bbox, dinormalisasi −1..+1:
  `x_norm = (cx / frame_width) * 2 - 1`
  (−1 = mentok kiri, 0 = tengah, +1 = mentok kanan). Inilah yang dipakai robot untuk
  meluruskan badan ke korban.
- **`area`** = luas bbox / luas frame (0..1). **Proksi jarak**: makin besar = makin dekat.
  Robot menganggap "sampai korban" saat `area` melewati ambang (kalibrasi di arena).
- **`conf`** = confidence (0..1).

> Estimasi jarak metrik opsional (focal length seperti kode lama) bisa ditambah, tapi untuk
> KRSRI cukup `x_norm` (untuk meluruskan) + `area` (untuk tahu sudah dekat). Lengan punya
> jangkauan tetap → robot cukup mendekat sampai `area` ambang lalu jalankan sekuens ambil.

---

## 7. Protokol komunikasi ke Teensy (UART)

- **Port Pi**: `/dev/ttyAMA0`, **115200 8N1**.
- **Format baris ASCII**, diakhiri `\n`, satu pesan per frame:

```
VIC <state> <x_norm> <area> <conf>\n
```

| Field | Tipe | Arti |
|---|---|---|
| `state` | int | `0` = tidak ada korban, `1` = terdeteksi |
| `x_norm`| float | −1.000 (kiri) … +1.000 (kanan) |
| `area`  | float | 0.000 … 1.000 (proksi jarak) |
| `conf`  | float | 0.000 … 1.000 |

Contoh:
```
VIC 1 -0.235 0.118 0.880
VIC 0 0.000 0.000 0.000
```

Kirim **~10–15 Hz** sudah cukup (jangan banjiri UART). Sertakan `state 0` saat tak ada
korban agar Teensy tahu harus mode mencari.

**Sisi Teensy** (yang perlu Anda tambahkan nanti di firmware — di luar scope README ini):
parser baris di `Serial2`, lalu state `M_ALIGN_VICTIM`:
- `x_norm < -tol` → `robot.walk(0, 0, +turn)` atau strafe kiri,
- `x_norm > +tol` → belok/strafe kanan,
- `|x_norm| ≤ tol` & `area ≥ ambang` → korban lurus & dekat → `enter(M_PICK_VICTIMx)`,
- `state == 0` lama → mode cari (putar pelan / lanjut wall-follow).

> Protokol ini pengganti modern dari sinyal lama `l/r/c` (`komponen.h::cekKamera`):
> sekarang kontinu & terukur, bukan perintah diskrit.

---

## 8. Instalasi di Raspberry Pi 5

```bash
sudo apt update && sudo apt install -y python3-pip python3-opencv
python3 -m venv ~/vision/venv
source ~/vision/venv/bin/activate
pip install onnxruntime numpy pyserial
# kamera CSI saja:
sudo apt install -y python3-picamera2
```

Aktifkan UART GPIO (sekali):
```bash
sudo raspi-config   # Interface Options -> Serial Port:
                    #   login shell over serial = NO
                    #   serial hardware = YES
# pastikan /boot/firmware/config.txt punya: enable_uart=1
sudo reboot
```
Verifikasi port: `/dev/ttyAMA0` (Pi 5). Tambahkan user ke grup: `sudo usermod -aG dialout $USER`.

Letakkan model di `~/vision/model/korban.onnx` dan skrip deteksi di `~/vision/detect.py`
(implementasi pipeline §5 — kode aplikasi, di luar scope README ini).

---

## 9. Menjalankan otomatis saat boot (systemd)

`/etc/systemd/system/vision.service`:
```ini
[Unit]
Description=Hexapod KRSRI Vision
After=multi-user.target

[Service]
ExecStart=/home/pi/vision/venv/bin/python /home/pi/vision/detect.py
WorkingDirectory=/home/pi/vision
User=pi
Restart=always

[Install]
WantedBy=multi-user.target
```
```bash
sudo systemctl enable --now vision.service
journalctl -u vision.service -f   # lihat log
```

---

## 10. Tuning performa di Pi 5

| Knob | Efek |
|---|---|
| `imgsz` 640 → 416 → 320 | FPS naik tajam, akurasi turun. Mulai **416**. |
| `onnxruntime` intra-op threads = 4 | manfaatkan semua core A76 |
| Frame skip (proses tiap N frame) | hemat CPU bila FPS kamera > kebutuhan |
| Resolusi capture kecil (mis. 640×480) | kurangi beban resize |
| `CONF_THRESH` 0.4–0.6 | naikkan bila banyak false positive arena |
| Hailo-8L (PCIe HAT) | opsional, lompatan FPS besar bila tersedia |

Target realistis: **15–30 FPS @ imgsz 416** di CPU Pi 5 — lebih dari cukup untuk
meluruskan robot ke korban.

---

## 11. Kalibrasi

1. **`CONF_THRESH`**: rekam video arena, naikkan sampai korban konsisten terdeteksi
   tanpa salah-deteksi dinding/objek lain.
2. **Ambang `area` "sampai korban"**: dekatkan robot ke korban pada jarak ideal lengan,
   catat nilai `area` di situ → jadikan ambang di sisi Teensy.
3. **`x_norm` tolerance**: seberapa lurus baru dianggap "centered" sebelum ambil
   (mis. ±0.08).
4. **Orientasi kamera**: pastikan sumbu `x_norm` searah dengan arah strafe/belok robot
   (uji: geser korban ke kanan frame → robot harus belok kanan).

---

## 12. Troubleshooting

| Gejala | Cek |
|---|---|
| Teensy tak terima data | port `/dev/ttyAMA0`, baud 115200, GND tersambung, level 3.3V |
| FPS rendah | turunkan `imgsz`, set threads=4, kecilkan resolusi capture, pasang fan |
| Korban tak terdeteksi | dataset kurang variasi → tambah data & latih ulang; turunkan `CONF_THRESH` |
| Banyak false positive | naikkan `CONF_THRESH`, tambah data negatif (dinding/objek arena) |
| bbox bergeser | preprocessing harus **letterbox**, dan `imgsz` Pi = `imgsz` ekspor ONNX |
| Robot belok ke arah salah | balik tanda `x_norm` atau perbaiki orientasi pemasangan kamera |

---

## 13. Ringkasan integrasi

1. Pi 5 deteksi korban → kirim `VIC <state> <x_norm> <area> <conf>\n` @~10–15Hz via UART.
2. Teensy `M_ALIGN_VICTIM`: luruskan badan pakai `x_norm`, dekati sampai `area` ambang.
3. Teensy `M_PICK_VICTIM`: jalankan sekuens lengan (`HexaArm`).
4. Lanjut FSM misi seperti biasa (`../HEXAPOD_KRSRI_2026/Mission.cpp`).

> Yang Pi lakukan: **lihat & beri tahu di mana korban**.
> Yang Teensy lakukan: **bergerak & mengambil**. Dua otak, satu kabel serial.
