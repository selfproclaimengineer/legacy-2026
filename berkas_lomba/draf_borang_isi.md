# Draf Isi Borang — Bagian yang Masih Kosong

Siap tempel ke `Borang Unlimited Legacy.docx`. Gaya prosa disamakan dengan bagian
Desain Robot yang sudah ada. Sesuaikan angka/merk bila berbeda dari perakitan akhir.

---

## SISTEM KENDALI DAN PENGGERAK

Arsitektur kendali robot ini menganut konsep dua-otak (dual-brain) yang memisahkan
tugas real-time dari tugas komputasi berat. Pengendali utama adalah **Teensy 4.1**
(mikrokontroler ARM Cortex-M7 600 MHz dengan FPU), yang bertanggung jawab atas seluruh
proses kritis-waktu: pembangkitan pola jalan (gait), penyelesaian kinematika balik
(inverse kinematics) untuk keenam kaki, stabilisasi badan, serta pengiriman sinyal
servo pada laju tetap 50 Hz secara non-blocking. Pemilihan Teensy 4.1 didasarkan pada
kecepatan dan unit FPU-nya yang mampu menghitung pose 18 derajat kebebasan tanpa
menimbulkan jeda gerak.

Sebagai ko-prosesor, robot menggunakan **Raspberry Pi 5** yang khusus menangani
pemrosesan citra (computer vision) untuk deteksi korban. Pembagian ini menjaga agar
beban inferensi neural network tidak mengganggu loop kendali gerak 50 Hz pada Teensy.
Komunikasi antara Raspberry Pi 5 dan Teensy dilakukan melalui jalur serial UART
(115200 bps) dengan protokol ringkas berisi status, kelas, dan posisi korban.

Penggerak gerak kaki menggunakan **18 motor servo digital metal-gear bertorsi tinggi**
(kelas RDS3235, ~35 kg·cm) — tiga per kaki untuk sendi Coxa, Femur, dan Tibia.
Pengendalian servo dilakukan oleh **dua modul driver PWM PCA9685** (16 kanal, antarmuka
I2C, alamat 0x40 dan 0x41) yang dikomando Teensy. Modul pertama menggerakkan kaki sisi
kanan beserta lengan penyelamat kanan, modul kedua menggerakkan kaki sisi kiri beserta
lengan penyelamat kiri. Setiap kanal PWM diatur pada frekuensi 50 Hz dengan rentang
pulsa terkalibrasi per servo (offset, arah, dan trim individual) untuk mengompensasi
variasi mekanik.

Untuk misi penyelamatan, robot dilengkapi **dua lengan penyelamat (gripper) mekanik**
(kanan dan kiri), masing-masing bersendi dan digerakkan oleh servo terpisah. Penggunaan
dua lengan memberikan fleksibilitas penjangkauan korban di kedua sisi lintasan dan
cengkeraman yang lebih andal pada korban berorientasi serong 45°. Kedua lengan dirancang
untuk mencengkeram korban tepat pada bagian bawah lengannya sesuai ketentuan, lalu
mengangkat dan memindahkannya ke zona aman. Dengan demikian total aktuator robot adalah
18 servo kaki ditambah servo kedua lengan penyelamat.

Sistem catu daya menggunakan baterai LiPo dengan papan distribusi daya (PDB) dan
regulator terpisah antara jalur logika (Teensy/Pi/sensor) dan jalur servo bertegangan
tinggi-arus, dengan ground yang disatukan. Mekanisme soft-start dan kapasitor penstabil
dipasang untuk mencegah brownout akibat lonjakan arus servo saat keenam kaki bergerak
serentak — aspek keandalan yang menjadi perhatian khusus pada robot berkaki berbeban.

---

## SISTEM SENSOR

Robot mengandalkan tiga lapis sistem sensor yang saling melengkapi untuk navigasi,
kestabilan, dan penyelesaian misi.

**1. Sensor jarak (navigasi & deteksi rintangan).** Robot menggunakan **enam sensor
ToF laser VL53L1X** (jangkauan hingga 4 m) yang diakses melalui **multiplexer I2C
TCA9548A** (alamat 0x70). Sensor ditempatkan menghadap depan, kedua sisi (kiri/kanan),
dan belakang untuk memungkinkan penelusuran dinding (wall-following), pengukuran lebar
lorong, serta pendeteksian mulut ruang korban dan zona aman. Mengingat tinggi dinding
arena hanya 10 cm dari lantai, sensor jarak dipasang pada ketinggian di bawah 10 cm agar
pancaran laser tepat mengenai bidang dinding. Pembacaan tiap sensor difilter secara
low-pass/Kalman untuk meredam derau, dan pemindaian dilakukan secara non-blocking
(round-robin) agar tidak mengganggu laju kendali gerak.

**2. Sensor orientasi (kestabilan & arah).** Robot menggunakan **IMU Yahboom 10-axis**
yang memadukan akselerometer 3-sumbu, giroskop 3-sumbu, magnetometer 3-sumbu, dan
sensor barometer/tekanan. Modul ini memiliki **algoritma Kalman bawaan** yang secara
stabil mengeluarkan sudut Euler (Roll, Pitch, Yaw) maupun quaternion dengan laju hingga
200 Hz melalui antarmuka UART (921600 bps)/USB/I2C. Casing logamnya membantu pelepasan
panas sekaligus **mengurangi interferensi medan magnet eksternal** terhadap magnetometer.
Roll dan Pitch digunakan untuk stabilisasi badan
secara aktif — robot mengoreksi kemiringan badannya saat melintasi jalan miring, jalan
pecah, jalan berpuing, dan jalan berlumpur agar pusat massa tetap terjaga. Keunggulan
konfigurasi 10-sumbu adalah adanya **magnetometer** yang memberikan **arah hadap
(heading) absolut layaknya kompas**, sehingga acuan orientasi tidak mengalami drift
seperti pada giroskop semata — sangat berguna menjaga arah robot pada rute arena yang
berbelok. Sensor barometer dapat menjadi acuan tambahan perubahan ketinggian saat
menaiki tangga/jalan miring. (Magnetometer dikalibrasi terhadap gangguan medan magnet
dari motor dan struktur logam robot.)

**3. Sistem penglihatan (deteksi & klasifikasi korban).** Kamera yang terhubung ke
**Raspberry Pi 5** menjalankan model deteksi objek **YOLOv8n yang dikonversi ke format
ONNX** dan diinferensikan dengan onnxruntime. Sistem ini tidak sekadar mendeteksi
keberadaan korban, tetapi **mengklasifikasikan korban asli versus korban dummy**
berdasarkan ciri pembeda utamanya, yaitu keberadaan lengan pada korban asli. Setelah
korban asli teridentifikasi, sistem menghitung posisi horizontalnya dalam bingkai kamera
(kiri/tengah/kanan) dan perkiraan kedekatannya, lalu mengirimkan informasi tersebut ke
Teensy melalui UART sebagai acuan robot meluruskan diri dan mendekati korban. Ketepatan
membedakan korban asli dari dummy ini menjadi inti strategi perolehan nilai
penyelamatan.

---

## ALGORITMA / STRATEGI

Strategi robot dibangun di atas prinsip **navigasi loop-tertutup (closed-loop)** yang
menggantikan pendekatan dead-reckoning, demi keandalan melintasi arena yang posisinya
diacak oleh juri.

**Navigasi.** Robot menelusuri lintasan menggunakan kombinasi *wall-following*
proporsional (menjaga jarak konstan ke dinding berdasarkan sensor jarak samping) dan
*heading-hold* berbasis kontrol PID terhadap Yaw IMU. Dengan demikian robot bergerak
lurus dan stabil mengikuti lorong selebar 45 cm, serta dapat berbelok presisi di
persimpangan. Sensor depan mendeteksi rintangan dan mulut ruang untuk memicu perpindahan
tahap misi.

**Pola jalan adaptif (multi-profil gait).** Robot menggunakan tripod gait dengan
trajektori kaki fase tumpu (stance) lurus berkecepatan konstan dan fase ayun (swing)
terangkat, sehingga tidak menyeret/tergelincir. Tersedia beberapa profil yang dipilih
secara otomatis sesuai medan: profil datar, profil tangga (untuk M2 dengan anak tangga
2 cm), profil merapat/*narrow* (untuk menyesuaikan lebar jejak kaki saat melalui R11
yang hanya menyisakan ~30 cm akibat tanah longsor), dan profil menunduk (*crouch*) untuk
menjangkau korban pada ruang rendah. Stabilisasi badan aktif berbasis IMU menjaga
kestabilan pada jalan miring, pecah, berpuing, dan berlumpur.

**Penyelesaian misi (state machine).** Logika misi disusun sebagai mesin-keadaan
(finite state machine) yang mengikuti urutan wajib lintasan (R1–R11 beserta K-1..K-5 dan
SZ-1..SZ-5). Setiap tahap memiliki syarat masuk, syarat keluar, serta batas-waktu dengan
mekanisme pemulihan (recovery) bila robot tersangkut. Untuk setiap misi penyelamatan,
robot: (1) mendeteksi dan memastikan korban **asli** melalui sistem penglihatan,
(2) meluruskan badan ke korban yang berorientasi serong 45°, (3) memilih lengan
penyelamat (kanan/kiri) yang sesuai sisi korban dan mencengkeram bagian bawah lengan
korban, (4) mengangkatnya keluar area korban, lalu (5) membawanya menuju zona aman
pasangannya. Ketersediaan dua lengan memungkinkan robot menangani korban di sisi mana
pun tanpa harus memutar badan penuh, menghemat waktu dan menjaga kestabilan.

**Strategi perolehan nilai.** Mengacu pada skema penilaian, robot memprioritaskan
**membawa korban menembus rintangan** karena bernilai 150 poin (dibanding 100 poin tanpa
korban), serta menargetkan **R9 sambil membawa korban (300 poin)** sebagai perolehan
tertinggi. Robot juga menjalankan rutin **pembersihan Safety Zone 3 dari batu koral
(R7)** menggunakan kaki/badan — bukan boneka — untuk meraih hingga 200 poin. Manajemen
waktu diarahkan untuk menyelesaikan kelima misi penyelamatan dan mencapai Finish dalam
kurang dari 300 detik guna memperoleh bonus pengali nilai (`total × 300 / waktu`), dengan
tetap mengutamakan keandalan di atas kecepatan agar tidak kehilangan poin akibat
kegagalan di medan sulit.

---

> Catatan pengisian:
> - Sesuaikan merk/seri servo & IMU dengan komponen final Anda.
> - Untuk **SKETSA GAMBAR DESAIN**: sertakan 6 tampak (isometrik, depan, kanan, kiri,
>   belakang, atas) lengkap dimensi, dan **jangan cantumkan nama institusi pada gambar**.
> - Jaga total proposal ≤ 25 halaman.
