//======================== Menuju Korban 1 =============================================
void menujuKorban1() {

  while (lid0 <= 20) {
    Serial.println(lid0);
    ikutUtara();
    followDindingKanan(13, 20);
    for (int i = 0; i < 18; i++) {
      passData(1);
    }
    Serial.println("1");
  }
}

//======================== Ambil Korban 1 ============================================
void ambilKorban1() {
  ikutBarat();
  for(int i =0;i<9;i++){
passData(0);
  }
  
  servo_ambil_korban_1();
}

//========================= Menuju Safe Zone 1 =======================================
void menujuSz1() {
  while (lid3 > 15 && lid4 < 200 && lid5 < 200) { 
    ikutUtara();
    followDindingKanan(12, 20);
    for (int i = 0; i < 18; i++) {
      passData(1);
    }
    Serial.println("2"); // Gerak maju
  }
}

//========================= Menuju Korban 2 ==========================================
void menujuKorban2() {
  while (lid4 < 30) {
    ikutSelatan();
    followDindingBelakang(12, 3);
    for (int i = 0; i < 18; i++) {
      passData(4);
    }
    Serial.println("3");
  }
}

//========================= Menuju Safe Zone 2 =======================================
void menujuSz2() {
  ikutTimur();
}

//========================= Fungsi Kontrol ===========================================
void kontol() {
  Serial.print("halo");
  for (int i = 0; i < 50; i++) {
    passData(4);
  }
}

//========================= Menuju Lantai 2 ==========================================
void menujuLantai2() {
  while (lid0 > 15) {
    Serial.println("4");
    ikutTimur();
    for (int i = 0; i < 18; i++) {
      passData(3);
    }
  }
  while (lid1 > 10) {
    Serial.println("5");
    ikutTimur();
    followDindingBelakang(10,3);
    for (int i = 0; i < 18; i++) {
      passData(4);
    }
  }
  while (lid3 > 15) {
    ikutTimur();
    for (int i = 0; i < 18; i++) {
      passData(1);
    }
  }
}

//========================= Ambil Korban 2 ===========================================
void ambilKorban2() {
  ikutBarat();
  Zoff -= 15;
  tinggiLangkah += 20;
  while (lid1 < 20) {
    ikutBarat();
    followDindingBelakang(12, 3);
    for (int i = 0; i < 18; i++) {
      passData(2);
    }
  }
  tinggiLangkah -= 20;
  Zoff += 15;
  for (int i = 0; i < 3; i++) {
    passData(0);
  }
  passData(0);
}

//========================= Menuju Safe Zone 3 =======================================
void menujuSz3() {
  ikutBarat();
  while (lid5 > 12) {
    // tampilkanData("lid5", lid5);
    // Serial.println(lid5);
    ikutBarat();
    followDindingBelakang(10, 3);
    for (int i = 0; i <=18; i++) {
      passData(2);
    }
  }
  passData(0);
}

//========================= Menuju Tangga ============================================
void menujuTangga() {
  ikutSelatan();
  while (lid1 > 15) {
    ikutSelatan();
    for (int i = 0; i < 18; i++) {
      passData(4);
    }
    passData(0);
  }
}

void naikTangga(){
  Yoff-=20;
  lebarLangkah+=20;
  tinggiLangkah+=20;
    while(lid5<40) {
    //delay(50);
    ikutSelatan();
    followDindingKanan(13, 20);
    for (int i = 0; i < 100; i++) {
      passData(1);
      //delay(10);
    }
    Serial.println("1");
    
  }
  tinggiLangkah-=20;
  lebarLangkah-=20;
  Yoff+=20;
  passData(0);

}
