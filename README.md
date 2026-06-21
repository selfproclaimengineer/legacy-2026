# legacy-2026 — Hexapod KRSRI

Robot hexapod untuk Kontes Robot SAR Indonesia (KRSRI). Repo ini berisi firmware
refaktor terbaru, subsistem vision, analisis lintas-generasi, dan arsip kode lama.

## Struktur

| Folder | Isi |
|---|---|
| [`HEXAPOD_KRSRI_2026/`](HEXAPOD_KRSRI_2026/) | **Firmware Teensy 4.1 (refaktor menyeluruh)** — gait+yaw, IK teruji, lidar non-blocking, lengan gripper, navigasi closed-loop, FSM misi. Lihat README-nya. |
| [`RASPI_VISION_KRSRI/`](RASPI_VISION_KRSRI/) | **Deteksi korban Raspberry Pi 5** (YOLOv8n → ONNX) + protokol UART ke Teensy. README lengkap. |
| [`program lama/ANALISIS.md`](program%20lama/ANALISIS.md) | Analisis mendalam 4 generasi kode + roadmap menang KRSRI. |
| `program lama/` | Arsip kode generasi lama (LEGACY2026, 2024, raspi vision). |

## Mulai dari mana

1. Baca [`program lama/ANALISIS.md`](program%20lama/ANALISIS.md) — apa yang salah di kode lama & kenapa.
2. [`HEXAPOD_KRSRI_2026/README.md`](HEXAPOD_KRSRI_2026/README.md) — arsitektur, wiring, kalibrasi, build.
3. [`RASPI_VISION_KRSRI/README.md`](RASPI_VISION_KRSRI/README.md) — sisi penglihatan.

Arsitektur: **Teensy** (real-time gerak 50Hz) + **Raspberry Pi 5** (vision) ↔ UART.
