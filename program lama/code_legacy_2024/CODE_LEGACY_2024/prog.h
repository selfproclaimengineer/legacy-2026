

//program utama
int cek = 0;
void followDindingKanan(int jarak, int lebar) {
  int toleransi = 5;
  int lamanya = 5;
  int selisihKanan = lid1 - lid2;  //15-20 = -5
  int selisihPositif = toleransi;
  int selisihNegatif = -1 * toleransi;
  int batasMendekatkan = jarak + toleransi;
  int batasMenjauhkan = jarak - toleransi;
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  while (((selisihKanan < selisihNegatif) && (lid2 > jarak)) && (lid1 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    for (int i = 0; i < lamanya; i++) passData(4);  //kanan
    Serial.println("serong tapi jauh");
  }
  while (((selisihKanan > selisihPositif) && (lid2 > jarak)) && (lid1 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    for (int i = 0; i < lamanya; i++) passData(4);  //kanan
    Serial.println("serong tapi jauh");
  }
  while (((selisihKanan < selisihNegatif) && (lid2 >= jarak)) && (lid1 <= jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
    Serial.println("serongkanan - 1");
  }
  // while (((selisihKanan < selisihNegatif) && (lid5 < jarak)) && (lid4 < jarak) && set == 0)  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  // {
  //   for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
  //   Serial.println("serongkanan - 2");
  // }
  while (((selisihKanan > selisihPositif) && (lid2 <= jarak)) && (lid1 >= jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
    Serial.println("serongKiri - 1");
  }
  // while (((selisihKanan > selisihPositif) && (lid5 < jarak)) && (lid4 < jarak) && set == 0)  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  // {
  //   for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
  //   Serial.println("serongKiri - 2");
  // }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  while ((lid2 > batasMendekatkan) && (lid1 > batasMendekatkan))  /////
  {
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (((selisihKanan < selisihNegatif) && (lid2 > jarak)) && (lid1 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(4);  //kanan
      Serial.println("serong tapi jauh");
    }
    while (((selisihKanan > selisihPositif) && (lid2 > jarak)) && (lid1 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(4);  //kanan
      Serial.println("serong tapi jauh");
    }
    while (((selisihKanan < selisihNegatif) && (lid2 >= jarak)) && (lid1 <= jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
      Serial.println("serongkanan - 1");
    }
    // while (((selisihKanan < selisihNegatif) && (lid5 < jarak)) && (lid4 < jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    // {
    //   for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
    //   Serial.println("serongkanan - 2");
    // }
    while (((selisihKanan > selisihPositif) && (lid2 <= jarak)) && (lid1 >= jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
      Serial.println("serongKiri - 1");
    }
    // while (((selisihKanan > selisihPositif) && (lid5 < jarak)) && (lid4 < jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    // {
    //   for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
    //   Serial.println("serongKiri - 2");
    // }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < lamanya; i++) passData(4);  //langkah kanan
    Serial.println("kekiri");
  }
  while ((lid2 < batasMenjauhkan) && (lid1 < batasMenjauhkan)) {
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (((selisihKanan < selisihNegatif) && (lid2 > jarak)) && (lid1 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(4);  //kanan
      Serial.println("serong tapi jauh");
    }
    while (((selisihKanan > selisihPositif) && (lid2 > jarak)) && (lid1 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(4);  //kanan
      Serial.println("serong tapi jauh");
    }
    while (((selisihKanan < selisihNegatif) && (lid2 >= jarak)) && (lid1 <= jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
      Serial.println("serongkanan - 1");
    }
    // while (((selisihKanan < selisihNegatif) && (lid5 < jarak)) && (lid4 < jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    // {
    //   for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
    //   Serial.println("serongkanan - 2");
    // }
    while (((selisihKanan > selisihPositif) && (lid2 <= jarak)) && (lid1 >= jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
      Serial.println("serongKiri - 1");
    }
    // while (((selisihKanan > selisihPositif) && (lid5 < jarak)) && (lid4 < jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    // {
    //   for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
    //   Serial.println("serongKiri - 2");
    // }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < lamanya; i++) passData(2);  //langkah kiri
    Serial.println("kekanan");
  }
  lebarLangkah = 40;
}
/*
void ikutiDindingKanan(int jarak) {
  int selisihKanan = lid4 - lid5;  //15-20 = -5
  int selisihPositif = 3;
  int selisihNegatif = -3;
  int batasMendekatkan = jarak + 2;
  int batasMenjauhkan = jarak - 2;
  while (((selisihKanan < selisihNegatif) && (lid5 > batasMendekatkan)) && (lid4 < batasMenjauhkan))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    passData(6);  //pivot kanan
    Serial.println("serongkanan");
  }
  while (((selisihKanan > selisihPositif) && (lid5 < batasMenjauhkan)) && (lid4 > batasMendekatkan))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    passData(5);  //pivot kiri
    Serial.println("serongKiri");
  }
  while ((lid5 > batasMendekatkan) && (lid4 > batasMendekatkan))  /////
  {
    passData(4);  //langkah kanan
    Serial.println("kekiri");
  }
  while ((lid5 < batasMenjauhkan) && (lid4 < batasMenjauhkan)) {
    passData(2);  //langkah kiri
    Serial.println("kekanan");
  }
}

void ikutiDindingKiri(int jarak) {
  int selisihKanan = lid2 - lid1;  //15-20 = -5
  int selisihPositif = 3;
  int selisihNegatif = -3;
  int batasMendekatkan = jarak + 2;
  int batasMenjauhkan = jarak - 2;
  while (((selisihKanan < selisihNegatif) && (lid1 > batasMendekatkan)) && (lid2 < batasMenjauhkan))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    passData(6);  //pivot kanan
    Serial.println("serongkanan");
  }
  while (((selisihKanan > selisihPositif) && (lid1 < batasMenjauhkan)) && (lid2 > batasMendekatkan))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    passData(5);  //pivot kiri
    Serial.println("serongKiri");
  }
  while ((lid1 > batasMendekatkan) && (lid2 > batasMendekatkan))  /////
  {
    passData(4);  //langkah kanan
    Serial.println("kekiri");
  }
  while ((lid1 < batasMenjauhkan) && (lid2 < batasMenjauhkan)) {
    passData(2);  //langkah kiri
    Serial.println("kekanan");
  }
}
*/
void followDindingKiri(int jarak, int lebar) {
  int toleransi = 3;
  int lamanya = 5;
  int selisihKanan = lid2 - lid1;  //15-20 = -5
  int selisihPositif = toleransi;
  int selisihNegatif = -1 * toleransi;
  int batasMendekatkan = jarak + toleransi;
  int batasMenjauhkan = jarak - toleransi;
  while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    for (int i = 0; i < lamanya; i++) passData(2);  //kanan
    Serial.println("serong tapi jauh");
  }
  while (((selisihKanan > selisihPositif) && (lid1 > jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    for (int i = 0; i < lamanya; i++) passData(2);  //kanan
    Serial.println("serong tapi jauh");
  }
  while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 < jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    lebarLangkah = lebar;
    for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
    Serial.println("serongKiri");
  }
  while (((selisihKanan > selisihPositif) && (lid1 < jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
  {
    lebarLangkah = lebar;
    for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
    Serial.println("serongkanan");
  }
  while ((lid1 > batasMendekatkan) && (lid2 > batasMendekatkan))  /////
  {
    while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(2);  //kanan
      Serial.println("serong tapi jauh");
    }
    while (((selisihKanan > selisihPositif) && (lid1 > jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(2);  //kanan
      Serial.println("serong tapi jauh");
    }
    while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 < jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      lebarLangkah = lebar;
      for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
      Serial.println("serongKiri");
    }
    while (((selisihKanan > selisihPositif) && (lid1 < jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      lebarLangkah = lebar;
      for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
      Serial.println("serongkanan");
    }
    lebarLangkah = 40;
    for (int i = 0; i < 20; i++) passData(2);  //langkah kiri
    Serial.println("kekiri");
  }
  while ((lid1 < batasMenjauhkan) && (lid2 < batasMenjauhkan)) {
    while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(2);  //kanan
      Serial.println("serong tapi jauh");
    }
    while (((selisihKanan > selisihPositif) && (lid1 > jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      for (int i = 0; i < lamanya; i++) passData(2);  //kanan
      Serial.println("serong tapi jauh");
    }
    while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 < jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      lebarLangkah = lebar;
      for (int i = 0; i < lamanya; i++) passData(5);  //pivot kiri
      Serial.println("serongKiri");
    }
    while (((selisihKanan > selisihPositif) && (lid1 < jarak)) && (lid2 > jarak))  // && (kanan0<batasDekat) && (kanan1<batasDekat))
    {
      lebarLangkah = lebar;
      for (int i = 0; i < lamanya; i++) passData(6);  //pivot kanan
      Serial.println("serongkanan");
    }
    lebarLangkah = 40;
    for (int i = 0; i < lamanya; i++) passData(4);  //langkah kanan
    Serial.println("kekanan");
  }
  lebarLangkah = 40;
}

void sesuaikanSz2() {
  followDindingKanan(43, 20);
  while (lid0 < 42) {
    followDindingKanan(43, 20);
    passData(3);  //kiri
  }
  while (lid0 > 46) {
    followDindingKanan(43, 20);
    passData(1);
  }
}

/*
void selesaiSz2() {
  while (lid4 > 50) {
    passData(4);  //kanan
    // followSelatan(10, 10, 25);
  }
  while (lid4 < 46) {
    // followSelatan(10, 10, 25);
    passData(2);  //kiri
  }
  while (lid0 < 18) {
    // followBarat(17, 10, 20);
    for (int i = 0; i < 35; i++) passData(3);  //kiri
  }
  while (lid0 > 22) {
    // followBarat(17, 10, 20);
    for (int i = 0; i < 35; i++) passData(1);
  }
}
*/

void sesuaikanSz1() {
  followDindingKiri(16, 20);
  while (lid3 < 41) {
    followDindingKiri(16, 20);
    passData(1);  //kiri
  }
  while (lid3 > 45) {
    followDindingKiri(16, 20);
    passData(3);
  }
}

void sesuaikanSz4() {
  while (lid4 > 32) {
    passData(4);  //kanan
    // followSelatan(10, 10, 25);
  }
  while (lid4 < 29) {
    // followSelatan(10, 10, 25);
    passData(2);  //kiri
  }
  while (lid0 < 63) {
    // followSelatan(10, 10, 25);
    passData(3);
  }
  while (lid0 > 67) {
    // followSelatan(10, 10, 25);
    passData(1);  //kiri
  }
}

void sesuaikanSz5() {
  while (lid1 > 22) {
    // followSelatan(10, 10, 25);
    passData(2);  //kiri
  }
  while (lid1 < 18) {
    // followSelatan(10, 10, 25);
    passData(4);  //kanan
  }
  while (lid3 < 26) {
    // followSelatan(10, 10, 25);
    passData(1);  //kiri
  }
  while (lid3 > 30) {
    // followSelatan(10, 10, 25);
    passData(3);
  }
}

void cekStart() {
  if (lid5 > 70 && lid4 > 70) {
    for (int i = 0; i < 50; i++) passData(6);
    Serial.println("hiii");
  } else if (lid1 > 70 && lid2 > 70) {
    for (int i = 0; i < 50; i++) passData(5);
    Serial.println("yo");
  } else if (lid3 > 70) {
    for (int i = 0; i < 100; i++) passData(5);
    Serial.println("jalskd;fjlkasdfj;lasdf");
  }
}
//////////////////////////////////////////////////////////////////////////
void menujuKorban1() {
  cekStart();
  while (lid3 < 55) {
    followDindingKanan(14, 15);
    for (int i = 0; i < 25; i++) passData(1);
  }
  for (int i = 0; i < 50; i++) passData(5);  //pivot kiri
  for (int i = 0; i < 50; i++) passData(3);
}


void ambilKorban1() {
  for (int i = 0; i < 20; i++) passData(50);
  servo_ambil_korban_0();
  cekKamera(8500);
  for (int i = 0; i < 20; i++) passData(3);
  for (int i = 0; i < 30; i++) passData(50);
  servo_pindah_korban();
}

void menujuSz1() {
  for (int i = 0; i < 50; i++) passData(6);
  followDindingKanan(14, 15);
  for (int i = 0; i < 550; i++) {
    tinggiLangkah = 120;
    followDindingKanan(14, 15);
    passData(1);
  }
  while (cek != 1) {
    tinggiLangkah = 120;
    if (bacaKompas() > 45) cek = 1;
    followDindingKanan(14, 15);
    passData(1);
  }
  while (cek != 2) {
    tinggiLangkah = 100;
    if ((lid0 < 28) && (lid2 > 21) && (bacaKompas() < 38)) cek = 2;
    followDindingKanan(14, 15);
    passData(1);
  }
  while (lid0 > 30) {
    followDindingKanan(16, 15);
    passData(1);
  }
  for (int i = 0; i < 100; i++) passData(6);
  followDindingKiri(16, 15);
  sesuaikanSz1();
}

void taruhKorban1() {
  for (int i = 0; i < 20; i++) passData(6);
  for (int i = 0; i < 20; i++) passData(50);
  servo_ambil_korban_1();
  for (int i = 0; i < 35; i++) passData(1);
  servo_park_home();
}

void menujuKorban2() {
  followDindingKiri(16, 15);
  lebarLangkah = 25;
  for (int i = 0; i < 20; i++) passData(3);
  while (lid3 > 15) {
    followDindingKiri(16, 15);
    lebarLangkah = 25;
    for (int i = 0; i < 20; i++) passData(3);
  }
  followDindingKiri(16, 15);
  Zoff = -110;
  posisiAwal = 60;
  lebarLangkah = 25;
  for (int i = 0; i < 70; i++) {
    while (lid3 > 15) {
      passData(3);
    }
    passData(4);
  }
  lebarLangkah = 25;
  while (lid5 > 73) {
    while (lid3 > 15) {
      for (int i = 0; i < 20; i++) passData(3);
    }
    passData(4);
  }
  while (lid0 < 100) {
    while (lid3 > 15) {
      for (int i = 0; i < 20; i++) passData(3);
    }
    passData(4);
  }
  lebarLangkah = 40;
  Zoff = -100;
  posisiAwal = 60;
}

void ambilKorban2() {
  servo_ambil_korban_0();
  cekKamera(8500);
}

void menujuSz2() {
  for (int i = 0; i < 55; i++) passData(6);
  followDindingKanan(16, 15);
}

void taruhKorban2() {
  for (int i = 0; i < 25; i++) passData(6);
  for (int i = 0; i < 20; i++) passData(50);
  servo_taruh_korban();
  for (int i = 0; i < 30; i++) passData(3);
  servo_park_home();
}

void menujuKorban3() {
  sesuaikanSz2();
  for (int i = 0; i < 50; i++) passData(5);
  followDindingKanan(16, 15);
  while (lid3 < 85) {
    followDindingKanan(16, 15);
    passData(1);
  }
  while ((lid1 > 20) && (lid3 > 40)) {
    followDindingKiri(16, 15);
  }
  followDindingKiri(16, 15);
  while (lid0 > 95) {
    Zoff = -110;
    posisiAwal = 60;
    followDindingKiri(16, 15);
    passData(1);
  }
  for (int i = 0; i < 70; i++) passData(6);
  for (int i = 0; i < 70; i++) passData(1);
}

void ambilKorban3() {
  servo_ambil_korban_0();
  cekKamera(8600);
  servo_ambil_korban_0_1();  //ambil korban
  for (int i = 0; i < 20; i++) passData(50);
  for (int i = 0; i < 20; i++) passData(3);
  servo_pindah_korban();  //pindah kobran
  delay(200);
  // servo_ambil_korban_0(); //ambil korban 4
  // delay(200);
}

void menujuKorban4() {
  for (int i = 0; i < 70; i++) passData(5);
  followDindingKiri(18, 20);
  followDindingKiri(18, 20);
  while (lid0 > 80) {
    Zoff = -110;
    posisiAwal = 60;
    followDindingKiri(18, 20);
    passData(1);
  }
  for (int i = 0; i < 50; i++) passData(6);
  for (int i = 0; i < 50; i++) passData(1);
}

void ambilKorban4() {
  servo_ambil_korban_0();
  cekKamera(10000);
  servo_ambil_korban_0_1();  //ambil korban
  servo_ambil_korban_0_3();  //pindah depan
  for (int i = 0; i < 20; i++) passData(50);
  for (int i = 0; i < 20; i++) passData(3);
}

void menujuSz3() {
  for (int i = 0; i < 50; i++) passData(5);
  followDindingKiri(16, 20);
  while (lid0 > 18) {
    Zoff = -110;
    posisiAwal = 60;
    followDindingKiri(16, 20);
    passData(1);
  }
  for (int i = 0; i < 50; i++) passData(6);
  for (int i = 0; i < 70; i++) passData(1);
  servo_ambil_korban_1();
  delay(500);
  servo_park_home_1();
  // servo_ambil_korban_1_2();
}

void menujuTangga() {
  Zoff = -100;
  posisiAwal = 60;
  for (int i = 0; i < 50; i++) passData(5);
  followDindingKanan(18, 20);
}

void naikTangga() {
  followDindingKanan(18, 15);
  while (lid1 > 30) {
    Zoff = -110;
    posisiAwal = 60;
    tinggiLangkah = 120;
    delayKecepatan = 10;
    followDindingKanan(18, 15);
    passData(1);
  }
  while ((lid2 < 149) && (lid1 < 149)) {
    Zoff = -115;
    posisiAwal = 68;
    lebarLangkah = 25;    //25
    tinggiLangkah = 115;  //115
    Yoff = -20;
    roll_x(-6);
    servo_naik_tangga();
    passData(1);
  }
}

void menujuSz4() {
  sesuaikanSz4();
}

void taruhKorban4() {
  sudut = bacaKompas();
  while (sudut < (selatan - 5)) {
    sudut = bacaKompas();
    passData(5);
  }
  for (int i = 0; i < 20; i++) passData(50);
  servo_taruh_korban();
  for (int i = 0; i < 30; i++) passData(3);
  servo_park_home();
}

void menujuKorban5() {
  followSelatan(7, 10, 25);
  sudut = bacaKompas();
  while (sudut < (selatan + 7)) {
    sudut = bacaKompas();
    passData(6);
  }
}

void ambilKorban5() {
  servo_ambil_korban_0();
  cekKamera(8500);
  servo_ambil_korban_0_1();
}

void menujuSz5() {
  followSelatan(7, 10, 25);
  while (lid0 < 63) {
    followSelatan(7, 10, 25);
    passData(3);
  }
  while (lid0 > 66) {
    followSelatan(7, 10, 25);
    passData(1);
  }
  while (lid1 > 25 && lid3 > 30) {
    followSelatan(7, 10, 25);
    passData(2);
  }
}

void taruhKorban5() {
  followDindingKiri(20, 20);
  sesuaikanSz5();
  for (int i = 0; i < 20; i++) passData(50);
  servo_taruh_korban();
  for (int i = 0; i < 30; i++) passData(3);
  servo_park_home();
}