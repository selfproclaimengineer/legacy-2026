unsigned long int time;

float gerakServo1(float nilai) {
  return map(nilai, 0, 180, 500, 2550);
}

void servo_taruh_korban() {
  kit1.writeMicroseconds(12, gerakServo1(0));
  kit1.writeMicroseconds(13, gerakServo(170));
  delay(300);
  kit1.writeMicroseconds(14, gerakServo(0));
}

void servo_park_home() {
  // Park home untuk robot 0
  kit0.writeMicroseconds(12, gerakServo(80));   // body 75
  kit0.writeMicroseconds(13, gerakServo(170));  // tengah
  kit0.writeMicroseconds(14, gerakServo1(0));   // capit

  // Park home untuk robot 1
  kit1.writeMicroseconds(12, gerakServo1(90));  // body
  kit1.writeMicroseconds(13, gerakServo1(80));  // tengah
  kit1.writeMicroseconds(14, gerakServo1(0));   // capit
}

void servo_park_home_1() {
  // Park home untuk robot 0
  kit0.writeMicroseconds(12, gerakServo(75));   // body
  kit0.writeMicroseconds(13, gerakServo(90));   // tengah
  kit0.writeMicroseconds(14, gerakServo1(50));  // capit
}

void servo_ambil_korban_0() {
  // Ambil korban untuk robot 0
  kit1.writeMicroseconds(12, gerakServo1(3));
  kit1.writeMicroseconds(13, gerakServo(175));
  delay(200);
  kit1.writeMicroseconds(14, gerakServo(0));
}

void servo_ambil_korban_0_2() {
  // Ambil korban untuk robot 0
  kit1.writeMicroseconds(12, gerakServo1(5));
  kit1.writeMicroseconds(13, gerakServo(180));
  delay(200);
  kit1.writeMicroseconds(14, gerakServo(0));
}

void servo_ambil_korban_0_1() {
  // Ambil korban untuk robot 1
  kit1.writeMicroseconds(14, gerakServo1(70));  // capit
  delay(300);
  kit1.writeMicroseconds(13, gerakServo(160));  // tengah
  delay(100);
  kit1.writeMicroseconds(12, gerakServo(175));  // body
}

void servo_ambil_korban_0_3() {
  // Ambil korban untuk robot 1
  kit1.writeMicroseconds(14, gerakServo1(70));  // capit
  delay(300);
  kit1.writeMicroseconds(13, gerakServo(145));  // tengah
  delay(100);
  kit1.writeMicroseconds(12, gerakServo(130));  // body
}

void servo_ambil_korban_1() {
  // Ambil korban untuk robot 1

  kit0.writeMicroseconds(12, gerakServo1(180));  // body
  delay(100);
  kit0.writeMicroseconds(13, gerakServo1(75));  // tengah
  delay(400);
  kit0.writeMicroseconds(14, gerakServo1(0));  // capit
}

void servo_ambil_korban_1_1() {
  // Ambil korban untuk robot 1
  kit0.writeMicroseconds(14, gerakServo1(50));  // capit
  delay(200);
  kit0.writeMicroseconds(13, gerakServo(170));  // tengah
  delay(200);
  kit0.writeMicroseconds(12, gerakServo(50));  // body
  //delay(100);
}

void servo_ambil_korban_1_2() {
  // Ambil korban untuk robot 1
  kit0.writeMicroseconds(14, gerakServo1(0));  // capit
  delay(200);
  kit0.writeMicroseconds(13, gerakServo(170));  // tengah
  delay(200);
  kit0.writeMicroseconds(12, gerakServo(50));  // body
  //delay(100);
}
void servo_pindah_korban() {
  // Pindah korban
  kit0.writeMicroseconds(14, gerakServo1(50));  // capit
  delay(200);
  kit1.writeMicroseconds(14, gerakServo(0));  //
  delay(200);
  kit1.writeMicroseconds(13, gerakServo1(100));
}

void servo_naik_tangga() {
  kit1.writeMicroseconds(12, gerakServo1(40));  // body
  kit1.writeMicroseconds(13, gerakServo1(80));  // tengah
  kit1.writeMicroseconds(14, gerakServo1(50));  // capit

  // kit0.writeMicroseconds(14, gerakServo1(0));  // capit
}

void cekKamera(int lama) {
  char keyInput;
  delay(1000);
  time = millis();
  Serial.println(millis()-time);
  while ((millis() - time) <= lama) {
    Serial.println(millis()-time);
    if (Serial.available() > 0) {
      keyInput = Serial.read();
      Serial.print("keyInput : ");
      Serial.println(keyInput);
      // cekOled(keyInput);
      switch (keyInput) {
        case 'l':  //kiri
          passData(2);
          break;
        case 'r':  //kanan
          passData(4);
          break;
        case 'c':  //maju
          passData(1);
          break;
      }
      Serial.flush();
    }
    // if (cekIr() == 0) {
    //   for (int i = 0; i > -50; i = i - 10) {
    //     Yoff = i;  //body mundur
    //     delay(5);
    //     passData(0);
    //   }
    //   delay(1000);
    //   break;
    // }
  }
  Serial.println("sudah");
  delayKecepatan = 0;
  if ((millis() - time) > lama) {
    for (int i = 0; i < 40; i++) passData(1);
  }
  servo_ambil_korban_0_1();  //ambil korban
  for (int i = 0; i < 50; i++) passData(50);
  time = 0;
}


/*
void cekKamera(int lama) {
  while (1) {
    uint16_t keyInput;
    int posisiTengahKorban = 330;
    int toleransiKorban = 20;
    // time = millis();
    // while (((millis() - time) <= lama)) {
    // Serial.print("time : ");
    // Serial.println(i);
    if (Serial.available()) {
      keyInput = Serial.parseInt();
      cekOled(keyInput);
      Serial.print("keyInput : ");
      Serial.println(keyInput);
      if (keyInput < posisiTengahKorban - toleransiKorban) {
        delayKecepatan = map(keyInput, 0, (posisiTengahKorban - toleransiKorban), 10, 0);
        passData(4);  //kanan
      } else if (keyInput > posisiTengahKorban + toleransiKorban) {
        delayKecepatan = map(keyInput, (posisiTengahKorban + toleransiKorban), 400, 0, 10);
        passData(2);  //kiri
      } else if (keyInput < posisiTengahKorban + toleransiKorban && keyInput > posisiTengahKorban - toleransiKorban) {
        delayKecepatan = 0;
        passData(1);
      }
    }
    if (cekIr() == 0) {
      for (int i = 0; i > -50; i = i - 10) {
        Yoff = i;  //body mundur
        delay(5);
      }
      break;
    }
    // }
    delayKecepatan = 0;
    // if ((millis() - time) > lama) {
    //   for (int i = 0; i < 40; i++) passData(1);
    // }
    // for (int i = 0; i < 50; i++) passData(50);
  }
}
*/
