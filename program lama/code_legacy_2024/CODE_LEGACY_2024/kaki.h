#include <Adafruit_PWMServoDriver.h>
#include <HardwareSerial.h>

// Alamat I2C untuk modul driver servo PCA9685
#define PCA9685_1_ADDR 0x40  // Alamat modul PCA9685 pertama, digunakan untuk mengontrol beberapa servo
#define PCA9685_2_ADDR 0x41  // Alamat modul PCA9685 kedua, digunakan jika jumlah servo lebih dari 16

// Inisialisasi driver PWM servo untuk masing-masing modul
Adafruit_PWMServoDriver kit0 = Adafruit_PWMServoDriver(PCA9685_1_ADDR);  // Driver pertama untuk kontrol servo
Adafruit_PWMServoDriver kit1 = Adafruit_PWMServoDriver(PCA9685_2_ADDR);  // Driver kedua untuk kontrol servo tambahan

// Panjang segmen kaki hexapod (dalam satuan mm)
#define coxa 20   // Panjang segmen Coxa (paha atas dari pangkal ke sendi pertama)  // waktu dibagi dengan
#define femur 60 // Panjang segmen Femur (paha bawah dari sendi pertama ke sendi kedua)
#define tibia 80 // Panjang segmen Tibia (betis dari sendi kedua ke ujung kaki)

// Ukuran dimensi tubuh hexapod (dalam satuan mm)
#define panjangBody 78  // Panjang tubuh dari depan ke belakang (body length)
#define lebarBody 90    // Lebar tubuh dari sisi ke sisi (body width)
#define lebarKecilBody 45  // Lebar bagian sempit tubuh, jika ada penyempitan pada bagian tengah

// Parameter gerakan kaki
#define rateKu 5  // Ketelitian langkah (semakin tinggi nilainya, semakin halus gerakan, namun harus ganjil)
                 // rateKu akan mempengaruhi pembagian langkah dan koordinasi

int lebarLangkah = 40;    // Lebar langkah per gerakan (dibagi dengan rateKu*2, hasilnya harus bilangan bulat)
                          // Misalnya, 40 dibagi (rateKu * 2) = 40 / 10 = 4 (hasil bulat, valid)
int tinggiLangkah = 70;  // Tinggi langkah kaki saat diangkat dari tanah (dibagi rateKu harus bulat)
                          // Misalnya, 100 / 5 = 20 (hasil bulat, valid)
int posisiAwal = 60;      // Posisi awal servo pada saat hexapod diam (dalam derajat servo)
int delayKecepatan = 0;   // Delay antara pergerakan kaki, nilai ini mengontrol kecepatan gerakan hexapod

// Offset posisi untuk menyesuaikan posisi awal atau referensi
int Zoff = -100;  // Offset pada sumbu Z (tinggi), menyesuaikan tinggi standar kaki di atas tanah
int Yoff = 0;     // Offset pada sumbu Y (horizontal kiri-kanan), menyesuaikan posisi kaki kiri/kanan
int Xoff = 0;     // Offset pada sumbu X (horizontal depan-belakang), menyesuaikan posisi kaki depan/belakang


// Variabel global
int input;  // Variabel untuk menyimpan input, mungkin dari sensor atau komunikasi
void syncLeg();  // Deklarasi fungsi untuk sinkronisasi gerakan kaki
void inverse(int idLeg, float x, float y, float z);  // Deklarasi fungsi untuk menghitung inverse kinematics tiap kaki

// Variabel yang digunakan untuk menentukan langkah kaki dalam gerakan "krepit" atau miring
// float posisiKepiting = ((coxa + femur) / 1.3);  // Posisi default saat hexapod bergerak menyamping, belum digunakan

float langkahNaik;  // Tinggi kaki saat diangkat dari tanah (belum diinisialisasi di bagian ini)
int jumlahPosisiKaki = rateKu * 2;  // Menghitung jumlah posisi kaki berdasarkan rateKu (dikalikan 2)
int rateKu1 = rateKu * 1;  // Variabel untuk menyimpan rateKu versi yang digandakan
int rateKu2 = rateKu * 2;  // Digunakan untuk menghitung posisi yang lebih halus
int rateKu3 = rateKu * 3;  // Digunakan untuk variasi gerakan lainnya
int rateKu4 = rateKu * 4;  // Tingkatan yang lebih tinggi untuk kontrol lebih presisi

int knopKaki = 0;  // Variabel untuk menandai langkah kaki saat ini, bisa digunakan dalam sinkronisasi gerakan
int indexKanan = 0;  // Indeks untuk kaki di sisi kanan hexapod, digunakan untuk mengatur pergerakan simultan
int indexKiri = 0;  // Indeks untuk kaki di sisi kiri hexapod
int inputBefore = 0;  // Menyimpan nilai input sebelumnya untuk deteksi perubahan input
int ubahGerak = 0;  // Menandakan apakah ada perubahan gerakan, bisa dipakai dalam logika kontrol gerakan

// Array untuk menyimpan posisi horizontal datar dari kaki saat melangkah
float langkahDatar[20];  // Array ini menyimpan posisi x untuk berbagai langkah kaki dalam satu siklus
// int bacaKakiPb[6];  // Dibaca dari pin atau sensor tertentu (mungkin potensiometer), tidak digunakan di sini
int selisih[6];  // Selisih posisi antar kaki yang bisa digunakan untuk sinkronisasi gerakan antar kaki
// int pb[6] = { 0, 0, 0, 0, 0, 0 };  // Digunakan untuk membaca nilai dari input lain, mungkin sensor, tapi belum digunakan

// Struktur data untuk menyimpan informasi tentang sistem servo dan kaki hexapod
struct sys {
  float pos;  // Posisi dari suatu komponen atau sudut
  float teta;  // Sudut rotasi atau orientasi
  float tet;   // Sudut rotasi lain, mungkin untuk sistem inverse kinematics
  int sel;  // Variabel untuk menyimpan nilai seleksi, digunakan dalam logika kontrol
  int prev_teta;  // Menyimpan sudut teta sebelumnya untuk perbandingan
  int angle;  // Sudut rotasi dari servo atau kaki
} sys[18];  // Array struct ini menyimpan informasi untuk 18 servo (3 servo per kaki, 6 kaki total)

// Struktur data untuk menyimpan informasi tentang masing-masing kaki hexapod
struct leg {
  int sudutDalam;  // Sudut Coxa (sendi pertama pada kaki)
  int sudutTengah;  // Sudut Femur (sendi kedua pada kaki)
  int sudutLuar;  // Sudut Tibia (sendi ketiga pada kaki)
  int posisi;  // Posisi keseluruhan kaki
  int motion;  // Jenis gerakan yang dilakukan kaki (misalnya, dorong atau angkat)
  int gerakan;  // Menyimpan tipe gerakan yang lebih spesifik

  int tinggi;  // Tinggi kaki saat diangkat
  int bukaan;  // Bukaan langkah atau jarak antara kaki satu dengan yang lain
  int putar;  // Nilai untuk rotasi kaki dalam sumbu horizontal (misalnya berputar atau melangkah ke samping)

  float posisiX;  // Posisi kaki di sumbu X (depan-belakang)
  float posisiY;  // Posisi kaki di sumbu Y (kiri-kanan)
  float posisiZ;  // Posisi kaki di sumbu Z (atas-bawah)
  
  // 0 = gerakan segitiga, kaki membentuk segitiga saat bergerak
  // 1 = gerakan dorong, kaki bergerak secara horizontal
} leg[6];  // Array struct untuk menyimpan informasi 6 kaki hexapod


//========Belum Selesai===========
/*
void inisialisasiAwal() {  //sudah ditambahi body kinematic
  for (int a = 0; a < 6; a++) {
    leg[0].posisiX = posisiAwal;
    leg[1].posisiX = posisiAwal + 50;
    leg[2].posisiX = posisiAwal;
    leg[3].posisiX = posisiAwal * (-1);
    leg[4].posisiX = (posisiAwal * (-1)) - 50;
    leg[5].posisiX = (posisiAwal * (-1));

    leg[0].posisiY = posisiAwal;
    leg[1].posisiY = 0;
    leg[2].posisiY = (posisiAwal * (-1));
    leg[3].posisiY = (posisiAwal * (-1));
    leg[4].posisiY = 0;
    leg[5].posisiY = posisiAwal;
    // tambahi program riset body kinematics
    leg[a].posisiZ = 0;
    leg[a].gerakan = 0;
    leg[a].posisi = jumlahPosisiKaki;
    inverse(a, leg[a].posisiX, leg[a].posisiY, leg[a].posisiZ);
  }
}
*/
//========Belum Selesai===========

//Code ini sudah disesuaikan dengan perubahan bodyKinematicsnya

void bodyKinematics(float rollAngle, float pitchAngle, float yawAngle) {
    // Terapkan rotasi tubuh berdasarkan sudut roll, pitch, dan yaw
    applyBodyRotation(rollAngle, pitchAngle, yawAngle);
    
    // Setelah posisi baru kaki dihitung dengan rotasi tubuh, kita perbarui posisi kaki berdasarkan hasilnya
    for (int i = 0; i < 6; i++) {
        // Posisi kaki sudah dihitung dalam applyBodyRotation
        leg[i].posisiX = body[i].posX;
        leg[i].posisiY = body[i].posY;
        leg[i].posisiZ = body[i].posZ;

        // Nilai gerakan default (dapat diubah untuk pola gerakan yang lebih kompleks)
        leg[i].gerakan = 0;

        // Jumlah posisi kaki dalam langkah (rateKu * 2 adalah konstanta yang sudah kita atur sebelumnya)
        leg[i].posisi = jumlahPosisiKaki;

        // Lakukan inverse kinematics untuk menghitung sudut servo dari posisi X, Y, Z kaki
        inverse(i, leg[i].posisiX, leg[i].posisiY, leg[i].posisiZ);
    }
}

//hari ini 03.10.2024


void setKaki() {
  // Kondisi untuk motion tangga (Yoff == -20)
  if (Yoff == -20) {  
    // Pengaturan rotasi kaki (putar)
    int putarValues[6] = {0, 0, 8, 0, 0, 0};
    // Pengaturan tinggi kaki
    int tinggiValues[6] = {0, 0, 0, 0, 0, 0};
    // Pengaturan bukaan kaki
    int bukaanValues[6] = {0, 0, 0, 0, 0, 0};

    // Terapkan pengaturan ke setiap kaki
    for (int i = 0; i < 6; i++) {
      leg[i].putar = putarValues[i];
      leg[i].tinggi = tinggiValues[i];
      leg[i].bukaan = bukaanValues[i];
    }
  } 
  // Kondisi default (Yoff != -20)
  else {
    // Pengaturan rotasi kaki (putar)
    int putarValues[6] = {0, 0, 0, 0, 0, 0};
    // Pengaturan tinggi kaki
    int tinggiValues[6] = {0, 0, 0, 0, 0, 0};
    // Pengaturan bukaan kaki
    int bukaanValues[6] = {0, 0, 0, 0, 0, 0};

    // Terapkan pengaturan ke setiap kaki
    for (int i = 0; i < 6; i++) {
      leg[i].putar = putarValues[i];
      leg[i].tinggi = tinggiValues[i];
      leg[i].bukaan = bukaanValues[i];
    }
  }
}


void setGerak() {
  langkahNaik = (tinggiLangkah / rateKu);
  // fuzzyLogic(knop, 1, 1.8);  //0 follow utara, 1 follow timur, 2 follow selatan, 3 follow barat
  langkahDatar[0] = (lebarLangkah / (rateKu * (2)));
  if (Yoff == -20) {
    langkahDatar[1] = (lebarLangkah / (rateKu * (1)));  //ke arah 1 kanan
    langkahDatar[2] = (lebarLangkah / (rateKu * (1.2)));    //ke arah 1 kiri
  } else {
    // langkahDatar[1] = (lebarLangkah / (rateKu * (1.4)));  //ke arah 1 kanan
    // langkahDatar[2] = (lebarLangkah / (rateKu * (1)));   //ke arah 1 kiri
    langkahDatar[1] = (lebarLangkah / (rateKu * (1)));  //ke arah 1 kanan
    langkahDatar[2] = (lebarLangkah / (rateKu * (1)));   //ke arah 1 kiri
  }
  //kiri
  langkahDatar[3] = (lebarLangkah / (rateKu * (1)));  //ke arah 4 kanan
  langkahDatar[4] = (lebarLangkah / (rateKu * (1)));  //ke arah 4 kiri

  //belakang
  langkahDatar[5] = (lebarLangkah / (rateKu * (1.25)));     //ke arah 2 kanan
  langkahDatar[6] = (lebarLangkah / (rateKu * (1)));  //ke arah 2 kiri

  //kanan
  langkahDatar[7] = (lebarLangkah / (rateKu * (1)));  //ke arah 3 kanan
  langkahDatar[8] = (lebarLangkah / (rateKu * (1)));  //ke arah 3 kiri

  //pivot
  if (Yoff == -20) {
    langkahDatar[9] = (lebarLangkah / (rateKu * (1.5)));  //pivot
  } else {
    langkahDatar[9] = (lebarLangkah / (rateKu * (1)));  //pivot
  }

  //wave gait
  // langkahDatar[10] = (lebarLangkah / (rateKu * (1.3)));  //ke arah 3 kanan
  // langkahDatar[11] = (lebarLangkah / (rateKu * (1.5)));  //ke arah 3 kiri
  // else if (pilih == 1) {                                     //fuzzylogic
  //   fuzzyLogic(0, 1.4, 1);                                     //0 follow utara, 1 follow timur, 2 follow selatan, 3 follow barat
  //   langkahDatar[1] = (lebarLangkah / (rateKu * (outKanan)));  //ke arah 1 kanan
  //   langkahDatar[2] = (lebarLangkah / (rateKu * (outKiri)));   //ke arah 1 kiri

  //   langkahDatar[5] = (lebarLangkah / (rateKu * (outKanan)));  //ke arah 2 kanan
  //   langkahDatar[6] = (lebarLangkah / (rateKu * (outKiri)));   //ke arah 2 kiri
  //   inputBefore = 1;
  // }
}

void setupKaki() {
  kit0.begin();
  kit1.begin();
  kit0.setPWMFreq(50);
  kit1.setPWMFreq(50);
  setKaki();
  setGerak();
  bodyKinematics(0.0, 0.0, 0.0);  // Jika ingin tidak ada rotasi
  syncLeg();
}

long rad2deg(float rad) {
  return ((int)(rad * 57.29577957855));
}

float gerakServo(float nilai) {
  return map(nilai, 0, 180, 500, 1800);
}

int float2int(float input) {
  int temp = (int)input;            // floor-rounded input
  float dif = input - (float)temp;  //
  if (dif < 0.5) return temp;
  else return temp + 1;
}

// void bacaPb() {
//   for (int j = 0; j < 6; j++) {
//     bacaKakiPb[j] = digitalRead(pb[j]);
//   }
// }

void syncWrite() {
  //program menyelaraskan kaki masih belum bisa diimplementasikan
  // for (int ang = 0; ang < 18; ang++) {
  //   sys[ang].prev_teta = sys[ang].teta;
  //   sys[ang].teta = sys[ang].angle;
  //   sys[ang].sel = sys[ang].teta - sys[ang].prev_teta;
  //   if (sys[ang].sel < 0) sys[ang].sel = sys[ang].sel * (-1);
  // }
  // int min_s = min(sys[0].sel, sys[1].sel);
  // for (int i = 2; i < 18; i++) {
  //   min_s = min(min_s, sys[i].sel);
  // }
  // if (min_s == 0) min_s = 1;
  // for (int i = 0; i <= min_s; i++) {
  //   for (int ang = 0; ang < 18; ang++) {
  //     sys[ang].tet = sys[ang].prev_teta + (((sys[ang].teta - sys[ang].prev_teta) / min_s) * i);
  //   }
  //   //kaki 0
  //   kit0.writeMicroseconds(11, gerakServo(sys[0].tet));
  //   kit0.writeMicroseconds(10, gerakServo(sys[1].tet));
  //   kit0.writeMicroseconds(9, gerakServo(sys[2].tet));
  //   //kaki 1
  //   kit0.writeMicroseconds(15, gerakServo(sys[3].tet));
  //   kit0.writeMicroseconds(14, gerakServo(sys[4].tet));
  //   kit0.writeMicroseconds(13, gerakServo(sys[5].tet));
  //   //kaki 2
  //   kit1.writeMicroseconds(0, gerakServo(sys[6].tet));
  //   kit1.writeMicroseconds(1, gerakServo(sys[7].tet));
  //   kit1.writeMicroseconds(2, gerakServo(sys[8].tet));
  //   //kaki 3
  //   kit1.writeMicroseconds(11, gerakServo(sys[9].tet));
  //   kit1.writeMicroseconds(10, gerakServo(sys[10].tet));
  //   kit1.writeMicroseconds(9, gerakServo(sys[11].tet));
  //   //kaki 4
  //   kit1.writeMicroseconds(13, gerakServo(sys[12].tet));
  //   kit1.writeMicroseconds(14, gerakServo(sys[13].tet));
  //   kit1.writeMicroseconds(15, gerakServo(sys[14].tet));
  //   //kaki 5
  //   kit0.writeMicroseconds(0, gerakServo(sys[15].tet));
  //   kit0.writeMicroseconds(1, gerakServo(sys[16].tet));
  //   kit0.writeMicroseconds(2, gerakServo(sys[17].tet));

  //delay(delayKecepatan);
  // // }
  // Serial.print("kanan :");
  // Serial.println(sys[8].angle);
  // Serial.print("kiri :");
  // Serial.println(sys[11].angle);
  //kaki 0
  kit0.writeMicroseconds(8, gerakServo(sys[0].angle));
  kit0.writeMicroseconds(9, gerakServo(sys[1].angle));
  kit0.writeMicroseconds(10, gerakServo(sys[2].angle));
  //kaki 1
  kit0.writeMicroseconds(4, gerakServo(sys[3].angle));
  kit0.writeMicroseconds(5, gerakServo(sys[4].angle));
  kit0.writeMicroseconds(6, gerakServo(sys[5].angle));
  // kit0.writeMicroseconds(7, gerakServo(sys[4].angle));
 
  //kaki 2
  kit0.writeMicroseconds(0, gerakServo(sys[6].angle));
  kit0.writeMicroseconds(1, gerakServo(sys[7].angle));
  kit0.writeMicroseconds(2, gerakServo(sys[8].angle));
  //kaki 3
  kit1.writeMicroseconds(8, gerakServo(sys[9].angle));
  kit1.writeMicroseconds(9, gerakServo(sys[10].angle));
  kit1.writeMicroseconds(10, gerakServo(sys[11].angle));
  //kaki 4
  kit1.writeMicroseconds(4, gerakServo(sys[12].angle));
  kit1.writeMicroseconds(5, gerakServo(sys[13].angle));
  kit1.writeMicroseconds(6, gerakServo(sys[14].angle));
  //kaki 5
  kit1.writeMicroseconds(0, gerakServo(sys[15].angle));
  kit1.writeMicroseconds(1, gerakServo(sys[16].angle));
  kit1.writeMicroseconds(2, gerakServo(sys[17].angle));
  delay(delayKecepatan);
}

void setServo(int idLeg, int sudut1, int sudut2, int sudut3) {
  if (idLeg == 0) {
    sys[0].angle = (90 - sudut1) + leg[idLeg].putar;
    sys[1].angle = (90 - sudut2) + leg[idLeg].tinggi;  //240
    sys[2].angle = (90 + sudut3) + leg[idLeg].bukaan;  //60
  } else if (idLeg == 1) {
    sys[3].angle = (90 - sudut1) + leg[idLeg].putar;
    sys[4].angle = (90 - sudut2) + leg[idLeg].tinggi;  //240
    sys[5].angle = (90 + sudut3) + leg[idLeg].bukaan;  //60
  } else if (idLeg == 2) {
    sys[6].angle = (90 - sudut1) + leg[idLeg].putar;
    sys[7].angle = (90 - sudut2) + leg[idLeg].tinggi;  //60
    sys[8].angle = (90 + sudut3) + leg[idLeg].bukaan;  //240
  } else if (idLeg == 3) {
    sys[9].angle = (270 - sudut1) + leg[idLeg].putar;
    sys[10].angle = (90 + sudut2) + leg[idLeg].tinggi;  //60
    sys[11].angle = (90 - sudut3) + leg[idLeg].bukaan;  //240
  } else if (idLeg == 4) {
    sys[12].angle = (270 - sudut1) + leg[idLeg].putar;
    sys[13].angle = (90 + sudut2) + leg[idLeg].tinggi;  //60
    sys[14].angle = (90 - sudut3) + leg[idLeg].bukaan;  //240
  } else if (idLeg == 5) {
    sys[15].angle = (270 - sudut1) + leg[idLeg].putar;
    sys[16].angle = (90 + sudut2) + leg[idLeg].tinggi;  //60
    sys[17].angle = (90 - sudut3) + leg[idLeg].bukaan;  //240
  }
  if(sys[idLeg].angle < 0) sys[idLeg].angle = 0;
  else if(sys[idLeg].angle > 180) sys[idLeg].angle = 180;
  // Serial.println(sys[15].angle);
}

void syncLeg() {
  setServo(0, leg[0].sudutDalam, leg[0].sudutTengah, leg[0].sudutLuar);
  setServo(1, leg[1].sudutDalam, leg[1].sudutTengah, leg[1].sudutLuar);
  setServo(2, leg[2].sudutDalam, leg[2].sudutTengah, leg[2].sudutLuar);
  setServo(3, leg[3].sudutDalam, leg[3].sudutTengah, leg[3].sudutLuar);
  setServo(4, leg[4].sudutDalam, leg[4].sudutTengah, leg[4].sudutLuar);
  setServo(5, leg[5].sudutDalam, leg[5].sudutTengah, leg[5].sudutLuar);
  syncWrite();
}

void inverse(int idLeg, float x, float y, float z) {
  float degree1, degree2, degree3;
  float degree2_1, degree2_2;
  float L1, L, L_2, A, Lcox;
  float temp2, temp2_2, temp2_3, temp2_4;
  float temp3, temp3_2, temp3_3, temp3_4;
  float femur_2, tibia_2;
  int sudut1, sudut2, sudut3;

  z = Zoff - z;
  degree1 = SpeedTrig.atan2(y, x);
  L1 = sqrt((x * x) + (y * y));  //akar x^2 + y^2

  Lcox = L1 - coxa;                   //coxa adalah 10
  L = sqrt((z * z) + (Lcox * Lcox));  //akar z^2 + Lcox^2
  femur_2 = femur * femur;
  tibia_2 = tibia * tibia;
  L_2 = L * L;

  degree2_1 = SpeedTrig.atan2(z, Lcox);
  temp2 = (((L_2 + femur_2) - tibia_2) / ((2 * femur) * L));
  degree2_2 = SpeedTrig.acos(temp2);
  degree2 = degree2_2 + degree2_1;

  temp3 = (((femur_2 + tibia_2) - L_2) / ((2 * femur) * tibia));
  degree3 = SpeedTrig.acos(temp3);

  sudut1 = float2int(rad2deg(degree1));
  sudut2 = float2int(rad2deg(degree2));
  sudut3 = float2int(rad2deg(degree3));
  if (x < 0 && y < 0) sudut1 = sudut1 + 360;
  if (L <= (femur + tibia)) sudut3 = sudut3 - 90;

  leg[idLeg].sudutDalam = sudut1;
  leg[idLeg].sudutTengah = sudut2;
  leg[idLeg].sudutLuar = sudut3;
}

void motion(int idLeg, int indexLebar)  //0,1
{
  if (leg[idLeg].motion == 0)  // ke Y positif
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      knopKaki = 1;
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }
  if (leg[idLeg].motion == 1 && knopKaki == 1)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }

  if (leg[idLeg].motion == 2)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
        // Serial.println("bbbb");
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      knopKaki = 2;
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
        // Serial.println("ccccccc");
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
        // Serial.println("d");
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }
  if (leg[idLeg].motion == 3 && knopKaki == 2)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      knopKaki = 2;
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
        // Serial.println("d");
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }

  //PIVOT KANAN
  if (leg[idLeg].motion == 4)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        if (idLeg == 0) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        if (idLeg == 0) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      knopKaki = 3;
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        if (idLeg == 0) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        if (idLeg == 0) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }

  if (leg[idLeg].motion == 5 && knopKaki == 3)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        if (idLeg == 3) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        if (idLeg == 3) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        if (idLeg == 3) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        if (idLeg == 3) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }

  //PIVOT KIRI
  if (leg[idLeg].motion == 6)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        if (idLeg == 0) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        if (idLeg == 0) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      knopKaki = 4;
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        if (idLeg == 0) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        if (idLeg == 0) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }

  if (leg[idLeg].motion == 7 && knopKaki == 4)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        if (idLeg == 3) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        if (idLeg == 3) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        if (idLeg == 3) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        if (idLeg == 3) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }

  //Kanan dan Kiri
  if ((leg[idLeg].motion == 8) || (leg[idLeg].motion == 10))  // ke Y positif
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        if (leg[idLeg].motion == 8) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 10) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        if (leg[idLeg].motion == 8) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 10) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      knopKaki = 5;
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        if (leg[idLeg].motion == 8) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 10) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        if (leg[idLeg].motion == 8) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 10) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }

  if ((leg[idLeg].motion == 9 || leg[idLeg].motion == 11) && knopKaki == 5)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }

  //Forward wave gate
  /*
  if (leg[idLeg].motion == 10) {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rateKu1) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      if (leg[idLeg].posisi < rateKu1)  //(rateKu+1))
      {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rateKu2) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rateKu2) leg[idLeg].gerakan = 0;
    }
  }
  */
  inverse(idLeg, leg[idLeg].posisiX, leg[idLeg].posisiY, leg[idLeg].posisiZ);
}


void directions() {
  if (inputBefore == 1) {  //MAJU
    indexKanan = 1;
    indexKiri = 2;
    leg[0].motion = 0;
    leg[1].motion = 1;
    leg[2].motion = 0;
    leg[3].motion = 1;
    leg[4].motion = 0;
    leg[5].motion = 1;
  } else if (inputBefore == 2) {  //KIRI
    indexKanan = 3;
    indexKiri = 4;
    leg[0].motion = 10;
    leg[1].motion = 11;
    leg[2].motion = 10;
    leg[3].motion = 11;
    leg[4].motion = 10;
    leg[5].motion = 11;
  } else if (inputBefore == 3) {  //MUNDUR
    indexKanan = 5;
    indexKiri = 6;
    leg[0].motion = 2;
    leg[1].motion = 3;
    leg[2].motion = 2;
    leg[3].motion = 3;
    leg[4].motion = 2;
    leg[5].motion = 3;
  } else if (inputBefore == 4) {  //KANAN
    indexKanan = 7;
    indexKiri = 8;
    leg[0].motion = 8;
    leg[1].motion = 9;
    leg[2].motion = 8;
    leg[3].motion = 9;
    leg[4].motion = 8;
    leg[5].motion = 9;
  } else if (inputBefore == 5) {  //pivot KIRI
    indexKanan = 9;
    indexKiri = 9;
    leg[0].motion = 6;
    leg[1].motion = 7;
    leg[2].motion = 6;
    leg[3].motion = 7;
    leg[4].motion = 6;
    leg[5].motion = 7;
  } else if (inputBefore == 6) {  //pivot KANAN
    indexKanan = 9;
    indexKiri = 9;
    leg[0].motion = 4;
    leg[1].motion = 5;  //3 sudah sesuai tap2nya
    leg[2].motion = 4;
    leg[3].motion = 5;
    leg[4].motion = 4;
    leg[5].motion = 5;
  }
  // else if (inputBefore == 7) {  //wave gait maju
  //   indexKanan = 10;
  //   indexKiri = 11;
  //   leg[0].motion = 4;
  //   leg[1].motion = 5;  //3 sudah sesuai tap2nya
  //   leg[2].motion = 4;
  //   leg[3].motion = 5;
  //   leg[4].motion = 4;
  //   leg[5].motion = 5;
  // }
  if (ubahGerak == 0) {
    setKaki();
    bodyKinematics(0.0, 0.0, 0.0);  // Jika ingin tidak ada rotasi

    syncLeg();
    ubahGerak = 1;
  }
  motion(0, indexKanan);
  motion(1, indexKanan);
  motion(2, indexKanan);
  motion(3, indexKiri);
  motion(4, indexKiri);
  motion(5, indexKiri);
  syncLeg();
}

void pasangKaki() {
  //kaki 0
   kit0.writeMicroseconds(8, gerakServo(sys[0].angle));
  kit0.writeMicroseconds(9, gerakServo(90));
  kit0.writeMicroseconds(10, gerakServo(180));
  //kaki 1
   kit0.writeMicroseconds(4, gerakServo(sys[3].angle));
  kit0.writeMicroseconds(5, gerakServo(90));
  kit0.writeMicroseconds(6, gerakServo(180));
  //kaki 2
   kit0.writeMicroseconds(0, gerakServo(sys[6].angle));
  kit0.writeMicroseconds(1, gerakServo(90));
  kit0.writeMicroseconds(2, gerakServo(180));
  //kaki 3
   kit1.writeMicroseconds(8, gerakServo(sys[9].angle));
  kit1.writeMicroseconds(9, gerakServo(90));
  kit1.writeMicroseconds(10, gerakServo(0));
  //kaki 4
   kit1.writeMicroseconds(4, gerakServo(sys[12].angle));
  kit1.writeMicroseconds(5, gerakServo(90));
  kit1.writeMicroseconds(6, gerakServo(0));
  //kaki 5
   kit1.writeMicroseconds(0, gerakServo(sys[15].angle));
  kit1.writeMicroseconds(1, gerakServo(90));
  kit1.writeMicroseconds(2, gerakServo(0));

  // kit0.writeMicroseconds(11, gerakServo(0));
  // kit0.writeMicroseconds(10, gerakServo(90));
  // kit0.writeMicroseconds(9, gerakServo(180));

  // kit1.writeMicroseconds(4, gerakServo(90));
  // kit1.writeMicroseconds(5, gerakServo(90));
  // kit1.writeMicroseconds(6, gerakServo(180));

  // kit1.writeMicroseconds(0, gerakServo(180));
  // kit1.writeMicroseconds(1, gerakServo(90));
  // kit1.writeMicroseconds(2, gerakServo(180));

  // kit1.writeMicroseconds(11, gerakServo(0));
  // kit1.writeMicroseconds(10, gerakServo(90));
  // kit1.writeMicroseconds(9, gerakServo(0));

  // kit1.writeMicroseconds(13, gerakServo(90));
  // kit1.writeMicroseconds(14, gerakServo(90));
  // kit1.writeMicroseconds(15, gerakServo(0));

  // kit0.writeMicroseconds(0, gerakServo(180));
  // kit0.writeMicroseconds(1, gerakServo(90));  //0
  // kit0.writeMicroseconds(2, gerakServo(0));
}

void passData(int masuk) {
  input = masuk;

  // Cek apakah posisi kaki sudah sesuai dengan rateKu2
  if (leg[0].posisi == rateKu2 || leg[1].posisi == rateKu2 || leg[2].posisi == rateKu2 || 
      leg[3].posisi == rateKu2 || leg[4].posisi == rateKu2 || leg[5].posisi == rateKu2) {
    
    if (input != inputBefore) {
      ubahGerak = 0;
      knopKaki = 0;
      // Memanggil bodyKinematics dengan rotasi nol
      bodyKinematics(0.0, 0.0, 0.0);  
      inputBefore = input;  // Simpan nilai input saat ini
    }
  }

  // Panggil fungsi untuk mengatur kaki
  setKaki();

  if (inputBefore == 0) {
    // Sistem tanpa closed-loop, panggil setGerak dan bodyKinematics
    setGerak();  
    bodyKinematics(0.0, 0.0, 0.0);  // Jika ingin tidak ada rotasi
    
    syncLeg();  // Sinkronkan gerakan kaki
  } else if (inputBefore == 50) {
    // Mengatur beberapa parameter default untuk posisi kaki dan tubuh
    Zoff = -100;
    lebarLangkah = 40;
    tinggiLangkah = 100;
    Yoff = 0;
    Xoff = 0;

    // Menghapus referensi ke variabel rotasi lama yang tidak diperlukan
    // Karena kita sudah menggunakan matriks rotasi, hapus bagian ini

    // Panggil setGerak dan bodyKinematics
    setGerak();
    bodyKinematics(0.0, 0.0, 0.0);  // Jika ingin tidak ada rotasi

    syncLeg();  // Sinkronkan gerakan kaki
  } else {
    // Panggil setGerak dan directions untuk mengontrol arah gerakan
    setGerak();
    directions();
  }
}

