# Uji matematika di PC (tanpa hardware)

IK adalah satu-satunya logika non-trivial yang bisa diverifikasi tanpa robot.
`test_kinematics.cpp` membuktikan: `IK(target) -> sudut -> FK(sudut)` kembali ke target,
dan target di luar jangkauan ditandai (bukan NaN/crash).

## Cara jalankan
Butuh `g++` (MinGW / WSL / Linux / macOS):

```bash
cd test
g++ -I.. test_kinematics.cpp ../LegIK.cpp -o t
./t
```

Output yang benar diakhiri `[PASS] semua kasus IK konsisten`.

> Catatan: formula IK ini sudah diverifikasi terpisah (round-trip error 0.0).
> Bila Anda mengubah `LegIK.cpp`, jalankan ulang test ini sebelum upload ke robot.

Jika tidak ada `g++` di Windows, instal lewat MSYS2/MinGW, atau pakai WSL.
