
#include <Adafruit_PWMServoDriver.h>
#include <HardwareSerial.h>

#define PCA9685_1_ADDR 0x40  // Alamat modul PCA9685 pertama
#define PCA9685_2_ADDR 0x41  // Alamat modul PCA9685 kedua

Adafruit_PWMServoDriver kit0 = Adafruit_PWMServoDriver(PCA9685_1_ADDR);
Adafruit_PWMServoDriver kit1 = Adafruit_PWMServoDriver(PCA9685_2_ADDR);

#define coxa 25  //30 //waktu dibagi dengan
#define femur 55
#define tibia 85
#define panjangBody 78
#define lebarBody 90
#define lebarKecilBody 45
#define rate 5            // ketelitian langkah  //harus ganjil
int lebarLangkah = 40;    // lebar langkah dibagi rate*2 harus bulat 40 4 32 //harus bulat agar stabil
int tinggiLangkah = 100;  //tinggi langkah dibagi rate harus bulat
#define posisiAwal 50
int delayKecepatan = 0;

int Zoff = -100;
int Yoff = 0;
int Xoff = 0;

int input = 0;
void syncLeg();
void inverse(int idLeg, float x, float y, float z);

// float posisiKepiting = ((coxa + femur) / 1.3);
float langkahNaik;
int jumlahPosisiKaki = rate * 2;  //terdapat pada void motion
int rate1 = rate * 1;
int rate2 = rate * 2;
int rate3 = rate * 3;
int rate4 = rate * 4;
int knopKaki = 0;
int indexKanan = 0;
int indexKiri = 0;
int inputBefore = 0;
int ubahGerak = 0;

float langkahDatar[20];
int bacaKakiPb[6];
int selisih[6];
int pb[6] = { 0, 0, 0, 0, 0, 0 };

struct sys {
  float pos, teta, tet;
  int sel, prev_teta, angle;
} sys[18];

struct leg {
  int sudutDalam, sudutTengah, sudutLuar, posisi, motion, gerakan;
  int tinggi, bukaan, putar;  // ada 4 motion
  float posisiX, posisiY, posisiZ;
  // 0 = gerakan segitiga     1 = gerakan dorong
} leg[6];
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
void bodyKinematics() {  //sudah ditambahi body kinematic
  for (int a = 0; a < 6; a++) {
    leg[0].posisiX = ((((posisiAwal + Xoff) + body[0].xRotX) + body[0].xRotY) + body[0].xRotZ);
    leg[1].posisiX = (((((posisiAwal + Xoff) + 25) + body[1].xRotX) + body[1].xRotY) + body[1].xRotZ);
    leg[2].posisiX = ((((posisiAwal + Xoff) + body[2].xRotX) + body[2].xRotY) + body[2].xRotZ);
    leg[3].posisiX = (((((posisiAwal * (-1)) + Xoff) + body[3].xRotX) + body[3].xRotY) + body[3].xRotZ);
    leg[4].posisiX = ((((((posisiAwal * (-1)) + Xoff) - 25) + body[4].xRotX) + body[4].xRotY) + body[4].xRotZ);
    leg[5].posisiX = (((((posisiAwal * (-1)) + Xoff) + body[5].xRotX) + body[5].xRotY) + body[5].xRotZ);

    leg[0].posisiY = ((((posisiAwal + Yoff) + body[0].yRotX) + body[0].yRotY) + body[0].yRotZ);
    leg[1].posisiY = ((((0 + Yoff) + body[1].yRotX) + body[1].yRotY) + body[1].yRotZ);
    leg[2].posisiY = ((((((posisiAwal * (-1)) + Yoff)) + body[2].yRotX) + body[2].yRotY) + body[2].yRotZ);
    leg[3].posisiY = ((((((posisiAwal * (-1)) + Yoff)) + body[3].yRotX) + body[3].yRotY) + body[3].yRotZ);
    leg[4].posisiY = ((((0 + Yoff) + body[4].yRotX) + body[4].yRotY) + body[4].yRotZ);
    leg[5].posisiY = ((((posisiAwal + Yoff) + body[5].yRotX) + body[5].yRotY) + body[5].yRotZ);

    leg[0].posisiZ = ((body[0].zRotX + body[0].zRotY) + body[0].zRotZ);
    leg[1].posisiZ = (((body[1].zRotX + body[1].zRotY) + body[1].zRotZ) - 0);
    leg[2].posisiZ = ((body[2].zRotX + body[2].zRotY) + body[2].zRotZ);
    leg[3].posisiZ = ((body[3].zRotX + body[3].zRotY) + body[3].zRotZ);
    leg[4].posisiZ = (((body[4].zRotX + body[4].zRotY) + body[4].zRotZ) - 0);
    leg[5].posisiZ = ((body[5].zRotX + body[5].zRotY) + body[5].zRotZ);
    leg[a].gerakan = 0;
    leg[a].posisi = jumlahPosisiKaki;
    inverse(a, leg[a].posisiX, leg[a].posisiY, leg[a].posisiZ);
  }
}

void setKaki() {
  if (Yoff == -20) { //20
    //PROGRAM INI DIGUNAKAN UNTUK MOTION TANGGA
    leg[0].putar = 5;   //kurang nilai ke depan
    leg[1].putar = 5;  //tambah ke depan
    leg[2].putar = 10;  //dikurang semakin kedepan
    //tambah nilai tambah maju
    leg[3].putar = 5;
    leg[4].putar = 5;  //tambah ke depan
    leg[5].putar = -5;

    //tambah nilai tambah turun
    leg[0].tinggi = 15;
    leg[1].tinggi = 10; //-10
    leg[2].tinggi = 5;
    //tambah nilai tambah naik
    leg[3].tinggi = -10;
    leg[4].tinggi = 10;
    leg[5].tinggi = 15;

    //tambah nilai tambah. buka
    leg[0].bukaan = -10;
    leg[1].bukaan = -5; //-10
    leg[2].bukaan = 10;
    //kurang nilai tambah buka
    leg[3].bukaan = 5 ; //0
    leg[4].bukaan = 10;
    leg[5].bukaan = 20;                                     
  } 
  /*
  if (Yoff == -20) {
    //PROGRAM INI DIGUNAKAN UNTUK MOTION TANGGA
    leg[0].putar = 10;   //kurang nilai ke depan
    leg[1].putar = 5;  //tambah ke depan
    leg[2].putar = 25;  //dikurang semakin kedepan
    //tambah nilai tambah maju
    leg[3].putar = -5;
    leg[4].putar = -5;  //tambah ke depan
    leg[5].putar = -25;

    //tambah nilai tambah turun
    leg[0].tinggi = 30;
    leg[1].tinggi = 25; //-10
    leg[2].tinggi = 5;
    //tambah nilai tambah naik
    leg[3].tinggi = -15;
    leg[4].tinggi = 0;
    leg[5].tinggi = -5;

    //tambah nilai tambah. buka
    leg[0].bukaan = 15;
    leg[1].bukaan = 10; //-10
    leg[2].bukaan = 25;
    //kurang nilai tambah buka
    leg[3].bukaan = 0;
    leg[4].bukaan = 0;
    leg[5].bukaan = 5;                                     
  } 
  */
  else {
    leg[0].putar = 0;  //kurang ke kepan
    leg[1].putar = 3;  //tambah ke depan //3
    leg[2].putar = 0;  //dikurang semakin kedepan
    //tambah nilai tambah maju
    leg[3].putar = 0;
    leg[4].putar = 0;   //tambah ke depan
    leg[5].putar = 10;  //tambah ke depan

    //tambah nilai tambah turun
    leg[0].tinggi = 15; //25
    leg[1].tinggi = 15;
    leg[2].tinggi = 10;
    //tambah nilai tambah naik
    leg[3].tinggi = -10;
    leg[4].tinggi = 0;
    leg[5].tinggi = 0; //-15

    //kurang nilai tambah tutup
    leg[0].bukaan = -10;
    leg[1].bukaan = -5;
    leg[2].bukaan = 15;
    //kurang nilai tambah buka
    leg[3].bukaan = 10;
    leg[4].bukaan = 0;
    leg[5].bukaan = 10;
  }
}

void setGerak() {
  langkahNaik = (tinggiLangkah / rate);;

  langkahDatar[0] = (lebarLangkah / (rate * (2)));
  if (Yoff == -20) {
    langkahDatar[1] = (lebarLangkah / (rate * (1.4)));  //ke arah 1 kanan
    langkahDatar[2] = (lebarLangkah / (rate * (1))); //ke arah 1 kiri
  } else {
    langkahDatar[1] = (lebarLangkah / (rate * (1)));     //ke arah 1 kanan
    langkahDatar[2] = (lebarLangkah / (rate * (1.4)));  //ke arah 1 kiri
  }
  //kiri
  langkahDatar[3] = (lebarLangkah / (rate * (1)));  //ke arah 4 kanan
  langkahDatar[4] = (lebarLangkah / (rate * (1)));  //ke arah 4 kiri

  //belakang
  langkahDatar[5] = (lebarLangkah / (rate * (1)));    //ke arah 2 kanan
  langkahDatar[6] = (lebarLangkah / (rate * (1.35)));  //ke arah 2 kiri

  //kanan
  langkahDatar[7] = (lebarLangkah / (rate * (1)));  //ke arah 3 kanan
  langkahDatar[8] = (lebarLangkah / (rate * (1)));  //ke arah 3 kiri

  //pivot
  if (Yoff == -20) {
    langkahDatar[9] = (lebarLangkah / (rate * (1.5)));  //pivot
  } else {
    langkahDatar[9] = (lebarLangkah / (rate * (1)));  //pivot
  }

  //wave gait
  // langkahDatar[10] = (lebarLangkah / (rate * (1.3)));  //ke arah 3 kanan
  // langkahDatar[11] = (lebarLangkah / (rate * (1.5)));  //ke arah 3 kiri
}

void setupKaki() {
  kit0.begin();
  kit1.begin();
  kit0.setPWMFreq(50);
  kit1.setPWMFreq(50);
  setKaki();
  setGerak();
  bodyKinematics();
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

void bacaPb() {
  for (int j = 0; j < 6; j++) {
    bacaKakiPb[j] = digitalRead(pb[j]);
  }
}

void syncWrite() {
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
  kit1.writeMicroseconds(8, gerakServo(sys[0].angle));
  kit1.writeMicroseconds(9, gerakServo(sys[1].angle));
  kit1.writeMicroseconds(10, gerakServo(sys[2].angle));
  //kaki 1
  kit1.writeMicroseconds(4, gerakServo(sys[3].angle));
  kit1.writeMicroseconds(5, gerakServo(sys[4].angle));
  // kit0.writeMicroseconds(7, gerakServo(sys[4].angle));
  kit1.writeMicroseconds(6, gerakServo(sys[5].angle));
  //kaki 2
  kit1.writeMicroseconds(0, gerakServo(sys[6].angle));
  kit1.writeMicroseconds(1, gerakServo(sys[7].angle));  
  kit1.writeMicroseconds(2, gerakServo(sys[8].angle));
  //kaki 3
  kit0.writeMicroseconds(8, gerakServo(sys[9].angle));
  kit0.writeMicroseconds(9, gerakServo(sys[10].angle));
  kit0.writeMicroseconds(10, gerakServo(sys[11].angle));
  //kaki 4
  kit0.writeMicroseconds(6, gerakServo(sys[12].angle));
  kit0.writeMicroseconds(5, gerakServo(sys[13].angle));
  kit0.writeMicroseconds(4, gerakServo(sys[14].angle));
  //kaki 5
  kit0.writeMicroseconds(0, gerakServo(sys[15].angle));
  kit0.writeMicroseconds(1, gerakServo(sys[16].angle));
  kit0.writeMicroseconds(2, gerakServo(sys[17].angle));
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
  // if(sys[idLeg].angle < 0) sys[idLeg].angle = 0;
  // else if(sys[idLeg].angle > 180) sys[idLeg].angle = 180;
  Serial.println(sys[15].angle);
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
      if (leg[idLeg].posisi > rate1) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
      {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }
  if (leg[idLeg].motion == 1 && knopKaki == 1)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
    } else if (leg[idLeg].gerakan == 1)  //asdf
    {
      if (leg[idLeg].posisi > 0) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ -= langkahNaik;
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == 0) leg[idLeg].gerakan = 2;
    } else if (leg[idLeg].gerakan == 2)  //gerak segitiga
    {
      if (leg[idLeg].posisi < rate1)  //(rate+1))
      {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }

  if (leg[idLeg].motion == 2)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
      {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
        // Serial.println("ccccccc");
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
        // Serial.println("d");
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }
  if (leg[idLeg].motion == 3 && knopKaki == 2)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
      {
        leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
        // Serial.println("d");
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }

  //PIVOT KANAN
  if (leg[idLeg].motion == 4)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        if (idLeg == 0) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
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
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        if (idLeg == 0) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }

  if (leg[idLeg].motion == 5 && knopKaki == 3)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        if (idLeg == 3) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
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
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        if (idLeg == 3) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }

  //PIVOT KIRI
  if (leg[idLeg].motion == 6)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        if (idLeg == 0) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
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
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        if (idLeg == 0) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 2) {
          leg[idLeg].posisiY -= langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 4) leg[idLeg].posisiY += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }

  if (leg[idLeg].motion == 7 && knopKaki == 4)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        if (idLeg == 3) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
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
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        if (idLeg == 3) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX -= langkahDatar[indexLebar];
        } else if (idLeg == 5) {
          leg[idLeg].posisiY += langkahDatar[indexLebar];
          leg[idLeg].posisiX += langkahDatar[indexLebar];
        } else if (idLeg == 1) leg[idLeg].posisiY -= langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }

  //Kanan dan Kiri
  if ((leg[idLeg].motion == 8) || (leg[idLeg].motion == 10))  // ke Y positif
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        if (leg[idLeg].motion == 8) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 10) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
      {
        if (leg[idLeg].motion == 8) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 10) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        if (leg[idLeg].motion == 8) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 10) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }

  if ((leg[idLeg].motion == 9 || leg[idLeg].motion == 11) && knopKaki == 5)  // ke Y positif++
  {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
      {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
    }
  }

  //Forward wave gate
  /*
  if (leg[idLeg].motion == 10) {
    if (leg[idLeg].gerakan == 0)  //dorong
    {
      if (leg[idLeg].posisi > rate1) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi--;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 1;
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
      if (leg[idLeg].posisi < rate1)  //(rate+1))
      {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX += langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        leg[idLeg].posisiZ += langkahNaik;
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate1) leg[idLeg].gerakan = 3;
    } else if (leg[idLeg].gerakan == 3)  //dorong
    {
      if (leg[idLeg].posisi < rate2) {
        if (leg[idLeg].motion == 9) leg[idLeg].posisiX -= langkahDatar[indexLebar];
        else if (leg[idLeg].motion == 11) leg[idLeg].posisiX += langkahDatar[indexLebar];
        leg[idLeg].posisi++;
      } else if (leg[idLeg].posisi == rate2) leg[idLeg].gerakan = 0;
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
    bodyKinematics();
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
  // kit0.writeMicroseconds(11, gerakServo(0));
  // kit0.writeMicroseconds(10, gerakServo(90));
  // kit0.writeMicroseconds(9, gerakServo(180));

  // kit1.writeMicroseconds(4, gerakServo(90));
  kit1.writeMicroseconds(5, gerakServo(90));
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
  if (leg[0].posisi == rate2 || leg[1].posisi == rate2 || leg[2].posisi == rate2 || leg[3].posisi == rate2 || leg[4].posisi == rate2 || leg[5].posisi == rate2) {
    if (input != inputBefore) {
      ubahGerak = 0;
      knopKaki = 0;
      bodyKinematics();
      inputBefore = input;
    }
  }

  if (inputBefore == 0) {
    setGerak();
    setKaki();
    bodyKinematics();
    syncLeg();
  } else if (inputBefore == 50) {
    Zoff = -100;
    lebarLangkah = 40;
    tinggiLangkah = 100;
    Yoff = 0;
    Xoff = 0;
    for (int i = 0; i < 6; i++) {
      body[i].xRotX = 0;
      body[i].yRotX = 0;
      body[i].zRotX = 0;
      body[i].xRotY = 0;
      body[i].yRotY = 0;
      body[i].zRotY = 0;
      body[i].xRotZ = 0;
      body[i].yRotZ = 0;
      body[i].zRotZ = 0;
    }
    setGerak();
    setKaki();
    bodyKinematics();
    syncLeg();
  } else {
    setKaki();
    setGerak();
    directions();
  }
}
