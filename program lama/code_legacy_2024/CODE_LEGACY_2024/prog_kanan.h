//program utama
int cek_1 = 0;

void sesuaikanSz2_1() {
  followDindingKiri(43, 20);
  while (lid0 < 42) {
    followDindingKiri(43, 20);
    passData(3);  //kiri
  }
  while (lid0 > 46) {
    followDindingKiri(43, 20);
    passData(1);
  }
}

/*
void selesaiSz2_1() {
  for (int i = 0; i < 70; i++) passData(1);
  while (lid0 > 20) {
    followDindingKiri(35,20);
    // followTimur(15, 10, 30);
    for (int i = 0; i < 35; i++) passData(1);
  }
}
*/

void sesuaikanSz1_1() {
  followDindingKanan(16, 20);
  while (lid3 < 41) {
    followDindingKanan(16, 20);
    passData(1);  //kiri
  }
  while (lid3 > 45) {
    followDindingKanan(16, 20);
    passData(3);
  }
}

void sesuaikanSz4_1() {
  while (lid2 > 32) {
    followSelatan(10, 10, 25);
    passData(2);  //kiri
  }
  while (lid2 < 29) {
    followSelatan(10, 10, 25);
    passData(4);  //kanan
  }
  while (lid0 < 63) {
    followSelatan(10, 10, 25);
    passData(3);
  }
  while (lid0 > 67) {
    followSelatan(10, 10, 25);
    passData(1);  //kiri
  }
}

void sesuaikanSz5_1() {
  while (lid5 > 22) {
    followSelatan(10, 10, 25);
    passData(4);  //kanan
  }
  while (lid5 < 18) {
    followSelatan(10, 10, 25);
    passData(2);  //kiri
  }
  while (lid3 < 26) {
    followSelatan(10, 10, 25);
    passData(1);  //kiri
  }
  while (lid3 > 30) {
    followSelatan(10, 10, 25);
    passData(3);
  }
}
///////////////////////////////////////////////////////////////
void menujuKorban1_1() {
  cekStart();
  while (lid3 < 65) {
    followDindingKiri(14, 15);
    for (int i = 0; i < 25; i++) passData(1);
  }
  followDindingKiri(14, 15);
  for (int i = 0; i < 50; i++) passData(6);  //pivot kiri
  // followDindingKanan(97); //pakai gak
  for (int i = 0; i < 30; i++) passData(3);
}

void ambilKorban1_1() {
  for (int i = 0; i < 10; i++) passData(50);
  servo_ambil_korban_0();
  cekKamera(8500);
  for (int i = 0; i < 20; i++) passData(3);
  for (int i = 0; i < 20; i++) passData(50);
  servo_pindah_korban();
}

void menujuSz1_1() {
  for (int i = 0; i < 50; i++) passData(5);
  followDindingKiri(14, 15);
  for (int i = 0; i < 550; i++) {
    tinggiLangkah = 120;
    followDindingKiri(14, 15);
    passData(1);
  }
  while (cek_1 != 1) {
    tinggiLangkah = 120;
    if (cekMpu() > 45) cek_1 = 1;
    followDindingKiri(14, 15);
    passData(1);
  }
  while (cek_1 != 2) {
    tinggiLangkah = 100;
    if ((lid0 < 28) && (lid4 > 21) && (cekMpu() < 38)) cek_1 = 2;
    followDindingKiri(14, 15);
    passData(1);
  }
  while (lid0 > 25) {
    followDindingKiri(16, 15);
    passData(1);
  }
  followDindingKiri(16, 15);
  for (int i = 0; i < 100; i++) passData(5);
  followDindingKanan(16, 15);
  sesuaikanSz1_1();
}

void taruhKorban1_1() {
  for (int i = 0; i < 20; i++) passData(5);
  for (int i = 0; i < 20; i++) passData(50);
  servo_ambil_korban_1();
  for (int i = 0; i < 35; i++) passData(1);
  servo_park_home();
}

void menujuKorban2_1() {
  sesuaikanSz1_1();
  followDindingKanan(14, 15);
  while (lid3 > 15) {
    followDindingKanan(14, 15);
    for (int i = 0; i < 20; i++) passData(3);
  }
  followDindingKanan(14, 15);
  for (int i = 0; i < 70; i++) {
    while (lid3 > 15) {
      passData(3);
    }
    passData(2);
  }
  while (lid1 > 80) {
    while (lid3 > 15) {
      passData(3);
    }
    passData(2);
  }
  // while (lid0 < 100) {
  //   while (lid3 > 15) {
  //     passData(3);
  //   }
  //   passData(2);
  // }
}

void ambilKorban2_1() {
  servo_ambil_korban_0();
  cekKamera(8500);
}

void menujuSz2_1() {
  for (int i = 0; i < 55; i++) passData(5);
  followDindingKiri(16, 15);
  for (int i = 0; i < 50; i++) passData(1);
}

void taruhKorban2_1() {  
  for (int i = 0; i < 20; i++) passData(5);
  for (int i = 0; i < 20; i++) passData(50);
  servo_taruh_korban();
  for (int i = 0; i < 30; i++) passData(3);
  servo_park_home();
}

void menujuKorban3_1() {
  followDindingKiri(16, 15);
  sesuaikanSz2_1();
  for (int i = 0; i < 50; i++) passData(6);
  followDindingKiri(16, 15);
  while (lid3 < 85) {
    followDindingKiri(16, 15);
    passData(1);
  }
  while ((lid5 > 20) && (lid3 > 25)) {
    followDindingKanan(20, 15);
  }
  followDindingKanan(16, 15);
  while (lid0 > 100) {
    Zoff = -110;
    posisiAwal = 65;
    followDindingKanan(16, 15);
    passData(1);
  }
  for (int i = 0; i < 50; i++) passData(5);
}

void ambilKorban3_1() {
  servo_ambil_korban_0();
  cekKamera(10000);
  servo_ambil_korban_0_1();  //ambil korban
  for (int i = 0; i < 20; i++) passData(50);
  for (int i = 0; i < 20; i++) passData(3);
  servo_pindah_korban();  //pindah kobran
  delay(200);
  //servo_ambil_korban_0(); //ambil korban 4
  //delay(200);
}

void menujuKorban4_1() {
  for (int i = 0; i < 50; i++) passData(6);
  followDindingKanan(16, 20);
  while (lid0 > 60) {
    Zoff = -110;
    posisiAwal = 65;
    followDindingKanan(16, 20);
    passData(1);
  }
  followDindingKanan(16, 20);
  for (int i = 0; i < 50; i++) passData(5);
}

void ambilKorban4_1() {
  servo_ambil_korban_0();
  cekKamera(10000);
  servo_ambil_korban_0_1();  //ambil korban
  servo_ambil_korban_0_3();  //pindah depan
  for (int i = 0; i < 20; i++) passData(50);
  for (int i = 0; i < 20; i++) passData(3);
}

void menujuSz3_1() {
  for (int i = 0; i < 50; i++) passData(6);
  followDindingKanan(16, 20);
  while (lid0 > 18) {
    Zoff = -110;
    posisiAwal = 65;
    followDindingKanan(16, 20);
    passData(1);
  }
  followDindingKanan(16, 20);
  for (int i = 0; i < 50; i++) passData(5);
  for (int i = 0; i < 50; i++) passData(1);
  servo_ambil_korban_1();
  delay(500);
  servo_park_home_1();
  // servo_ambil_korban_1_2();
}

void menujuTangga_1() {
  Zoff = -100;
  posisiAwal = 60;
  for (int i = 0; i < 50; i++) passData(6);
  followDindingKiri(18, 20);
}

void naikTangga_1() {
  followDindingKiri(18, 15);
  while (lid5 > 30) {
    Zoff = -110;
    posisiAwal = 65;
    tinggiLangkah = 120;
    delayKecepatan = 10;
    followDindingKiri(18, 15);
    passData(1);
  }
  while ((lid4 < 149) && (lid5 < 149)) {
    Zoff = -115;
    posisiAwal = 68;
    lebarLangkah = 25;    //25
    tinggiLangkah = 115;  //115
    Yoff = -10;
    roll_x(-6);
    servo_naik_tangga();
    passData(1);
  }
}

void menujuSz4_1() {
  sesuaikanSz4_1();
}

void taruhKorban4_1() {
  sudut = bacaKompas();
  while (sudut < (selatan + 5)) {
    sudut = bacaKompas();
    passData(6);
  }
  for (int i = 0; i < 20; i++) passData(50);
  servo_taruh_korban();
  for (int i = 0; i < 30; i++) passData(3);
  servo_park_home();
}

void menujuKorban5_1() {
  // followSelatan(7, 10, 25);
  sudut = bacaKompas();
  while (sudut < (selatan - 7)) {
    sudut = bacaKompas();
    passData(5);
  }
}

void ambilKorban5_1() {
  servo_ambil_korban_0();
  cekKamera(8500);
  servo_ambil_korban_0_1();
}

void menujuSz5_1() {
  // followSelatan(7, 10, 25);
  while (lid0 < 63) {
    // followSelatan(7, 10, 25);
    passData(3);
  }
  while (lid0 > 66) {
    // followSelatan(7, 10, 25);
    passData(1);
  }
  while (lid1 > 25 && lid3 > 30) {
    // followSelatan(7, 10, 25);
    passData(4);
  }
}

void taruhKorban5_1() {
  followDindingKanan(20, 20);
  sesuaikanSz5_1();
  for (int i = 0; i < 20; i++) passData(50);
  servo_taruh_korban();
  for (int i = 0; i < 30; i++) passData(3);
  servo_park_home();
}