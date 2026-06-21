int cek = 0;


// 
// int selatan = 185;
// int barat = 100;
// int timur = 260 ; 

void followDindingKanan(int jarak, int lebar) {
  int toleransi = 3;
  int lamanya = 3;
  int selisihKanan = lid1 - lid2;  // 15-20 = -5
  int selisihPositif = toleransi;
  int selisihNegatif = -1 * toleransi;
  int batasMendekatkan = jarak + toleransi;
  int batasMenjauhkan = jarak - toleransi;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  while (((selisihKanan < selisihNegatif) && (lid2 > jarak)) && (lid1 > jarak)) {
    for (int i = 0; i < lamanya; i++) passData(4);  // kanan
    Serial.println("serong tapi jauh");
  }

  while (((selisihKanan > selisihPositif) && (lid2 > jarak)) && (lid1 > jarak)) {
    for (int i = 0; i < lamanya; i++) passData(4);  // kanan
    Serial.println("serong tapi jauh");
  }

  while (((selisihKanan < selisihNegatif) && (lid2 >= jarak)) && (lid1 <= jarak)) {
    for (int i = 0; i < lamanya; i++) passData(6);  // pivot kanan
    Serial.println("serongkanan - 1");
  }

  while (((selisihKanan > selisihPositif) && (lid2 <= jarak)) && (lid1 >= jarak)) {
    for (int i = 0; i < lamanya; i++) passData(5);  // pivot kiri
    Serial.println("serongKiri - 1");
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  while ((lid2 > batasMendekatkan) && (lid1 > batasMendekatkan)) {
    while (((selisihKanan < selisihNegatif) && (lid2 > jarak)) && (lid1 > jarak)) {
      for (int i = 0; i < lamanya; i++) passData(4);  // kanan
      Serial.println("serong tapi jauh");
    }

    while (((selisihKanan > selisihPositif) && (lid2 > jarak)) && (lid1 > jarak)) {
      for (int i = 0; i < lamanya; i++) passData(4);  // kanan
      Serial.println("serong tapi jauh");
    }

    while (((selisihKanan < selisihNegatif) && (lid2 >= jarak)) && (lid1 <= jarak)) {
      for (int i = 0; i < lamanya; i++) passData(6);  // pivot kanan
      Serial.println("serongkanan - 1");
    }

    while (((selisihKanan > selisihPositif) && (lid2 <= jarak)) && (lid1 >= jarak)) {
      for (int i = 0; i < lamanya; i++) passData(5);  // pivot kiri
      Serial.println("serongKiri - 1");
    }

    for (int i = 0; i < lamanya; i++) passData(4);  // langkah kanan
    Serial.println("kekiri");
  }

  while ((lid2 < batasMenjauhkan) && (lid1 < batasMenjauhkan)) {
    while (((selisihKanan < selisihNegatif) && (lid2 > jarak)) && (lid1 > jarak)) {
      for (int i = 0; i < lamanya; i++) passData(4);  // kanan
      Serial.println("serong tapi jauh");
    }

    while (((selisihKanan > selisihPositif) && (lid2 > jarak)) && (lid1 > jarak)) {
      for (int i = 0; i < lamanya; i++) passData(4);  // kanan
      Serial.println("serong tapi jauh");
    }

    while (((selisihKanan < selisihNegatif) && (lid2 >= jarak)) && (lid1 <= jarak)) {
      for (int i = 0; i < lamanya; i++) passData(6);  // pivot kanan
      Serial.println("serongkanan - 1");
    }

    while (((selisihKanan > selisihPositif) && (lid2 <= jarak)) && (lid1 >= jarak)) {
      for (int i = 0; i < lamanya; i++) passData(5);  // pivot kiri
      Serial.println("serongKiri - 1");
    }

    for (int i = 0; i < lamanya; i++) passData(2);  // langkah kiri
    Serial.println("kekanan");
  }

  lebarLangkah = 40;
}

void followDindingKiri(int jarak, int lebar) {
  int toleransi = 3;
  int lamanya = 5;
  int selisihKanan = lid2 - lid1;  // 15-20 = -5
  int selisihPositif = toleransi;
  int selisihNegatif = -1 * toleransi;
  int batasMendekatkan = jarak + toleransi;
  int batasMenjauhkan = jarak - toleransi;

  while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 > jarak)) {
    for (int i = 0; i < lamanya; i++) passData(2);  // kanan
    Serial.println("serong tapi jauh");
  }

  while (((selisihKanan > selisihPositif) && (lid1 > jarak)) && (lid2 > jarak)) {
    for (int i = 0; i < lamanya; i++) passData(2);  // kanan
    Serial.println("serong tapi jauh");
  }

  while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 < jarak)) {
    lebarLangkah = lebar;
    for (int i = 0; i < lamanya; i++) passData(5);  // pivot kiri
    Serial.println("serongKiri");
  }

  while (((selisihKanan > selisihPositif) && (lid1 < jarak)) && (lid2 > jarak)) {
    lebarLangkah = lebar;
    for (int i = 0; i < lamanya; i++) passData(6);  // pivot kanan
    Serial.println("serongkanan");
  }

  while ((lid1 > batasMendekatkan) && (lid2 > batasMendekatkan)) {
    while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 > jarak)) {
      for (int i = 0; i < lamanya; i++) passData(2);  // kanan
      Serial.println("serong tapi jauh");
    }

    while (((selisihKanan > selisihPositif) && (lid1 > jarak)) && (lid2 > jarak)) {
      for (int i = 0; i < lamanya; i++) passData(2);  // kanan
      Serial.println("serong tapi jauh");
    }

    while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 < jarak)) {
      lebarLangkah = lebar;
      for (int i = 0; i < lamanya; i++) passData(5);  // pivot kiri
      Serial.println("serongKiri");
    }

    while (((selisihKanan > selisihPositif) && (lid1 < jarak)) && (lid2 > jarak)) {
      lebarLangkah = lebar;
      for (int i = 0; i < lamanya; i++) passData(6);  // pivot kanan
      Serial.println("serongkanan");
    }

    lebarLangkah = 40;
    for (int i = 0; i < 20; i++) passData(2);  // langkah kiri
    Serial.println("kekiri");
  }

  while ((lid1 < batasMenjauhkan) && (lid2 < batasMenjauhkan)) {
    while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 > jarak)) {
      for (int i = 0; i < lamanya; i++) passData(2);  // kanan
      Serial.println("serong tapi jauh");
    }

    while (((selisihKanan > selisihPositif) && (lid1 > jarak)) && (lid2 > jarak)) {
      for (int i = 0; i < lamanya; i++) passData(2);  // kanan
      Serial.println("serong tapi jauh");
    }

    while (((selisihKanan < selisihNegatif) && (lid1 > jarak)) && (lid2 < jarak)) {
      lebarLangkah = lebar;
      for (int i = 0; i < lamanya; i++) passData(5);  // pivot kiri
      Serial.println("serongKiri");
    }

    while (((selisihKanan > selisihPositif) && (lid1 < jarak)) && (lid2 > jarak)) {
      lebarLangkah = lebar;
      for (int i = 0; i < lamanya; i++) passData(6);  // pivot kanan
      Serial.println("serongkanan");
    }

    lebarLangkah = 40;
    for (int i = 0; i < lamanya; i++) passData(4);  // langkah kanan
    Serial.println("kekanan");
  }

  lebarLangkah = 40;
}

void ikutBarat(int head){
   while(true){
    int sudut = bacaKompas();
  if(sudut > 65 && sudut < 75){
    break;
    passData(0);
  }
  for (int i = 0; i < 3; i++) passData(head);
  }
}

void ikutSelatan(int head){
   while(true){
    int sudut = bacaKompas();
  if(sudut > 170 && sudut <180){
    break;
    passData(0);
  }
  for (int i = 0; i < 3; i++) passData(head);
  }
}

void ikutTimur(int head){
   while(true){
    int sudut = bacaKompas();
  if(sudut > 255 && sudut < 265){
    break;
    passData(0);
  }
  for (int i = 0; i < 3; i++) passData(head);
  }
}

void ikutUtara(){
while(true){
    int sudut = bacaKompas();
  if(sudut >= 340 && sudut <= 360){
    break;
    passData(0);
  }
  else if(sudut < 170 && sudut >= 0 ){
  for (int i = 0; i < 3; i++){
    passData(6);
  }
  }
  // else if(sudut < 360 && sudut >= 340 ){
  // for (int i = 0; i < 3; i++){
  //   passData(6);
  // }

  // }
   else if( sudut >= 170 && sudut < 340 ){
    for(int i=0; i<3;i++){
      passData(5);
    }
    }

  }
 
  }
 

//========================Menuju Korban 1=============================================
void menujuKorban1() {
  ikutUtara();
  while (lid0 < 25) {
    followDindingKanan(13, 20);
    for (int i = 0;i<3;i++){
      passData(1); 
  }
}
}

//========================ambil korban 1================================

void ambilKorban1() {
  ikutBarat(5);
}
  
//=========================Menuju Safe Zone 1============================
void menujuSz1() {
  ikutUtara();
  while (lid3 > 10 && lid4 <200) { 
    followDindingKanan(13, 20);
    for(int i = 0;i<20;i++) passData(1);  // Gerak maju
    // ikutUtara(5);
  }
}
