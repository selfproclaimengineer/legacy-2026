#include "bodyKinematics.h"
#include "kaki.h"
#include "module.h"
#include "algorithm.h"

int j, k, l, m, n, o, p;
char keyInput;

void setup() {
  Serial.begin(115200);
  setupKompas();
  setupKaki();
  setupBody();
  servo_park_home();
  passData(0);
  k = 180;
  l = 180;
}

void loop() {
  // for (int i = 0; i < 50; i++) {

  // }
  // for (int i = 0; i < 50; i++) {
  // passData(1);
  // }
  // for (int i = 0; i < 50; i++) {
  //   passData(5);
  // }2
  // for (int i = 0; i < 50; i++) {
  //   passData(6);
  // }
  // delay(1000);
  // keyInput = 'g';
  // bacaKompas();
  if (Serial.available() > 0) {
    keyInput = Serial.read();
    Serial.print("keyInput : ");
    Serial.println(keyInput);
    switch (keyInput) {
      case 'a':  //kiri
        input = 2;
        break;
      case 'd':  //kanan
        input = 4;
        break;
      case 'e':  //pivot kanan
        input = 6;
        break;
      case 'f':  //tambah
        Zoff = -70;
        lebarLangkah = 40;
        tinggiLangkah = 100;
        input = 0;
        break;
      case 'g':
        //PROGRAM KEDEPAN
        Zoff = -115;
        lebarLangkah = 25;    //25
        tinggiLangkah = 115;  //115
        // delayKecepatan = 7;
        Yoff = -20;
        roll_x(-6);

        kit1.writeMicroseconds(12, gerakServo1(40));  // body
        kit1.writeMicroseconds(13, gerakServo1(80));  // tengah
        kit1.writeMicroseconds(14, gerakServo1(50));  // capit

        kit0.writeMicroseconds(14, gerakServo1(0));  // capit

        input = 0;
        break;
      case 'h':             //aku ubah jadi motion tangga de
        servo_park_home();  //servo park capit bawah
        input = 0;
        break;
      case 'i':  // capit turun
        servo_ambil_korban_0_1();
        input = 0;
        break;
      case 'j':
        kit1.writeMicroseconds(12, gerakServo1(0));
        kit1.writeMicroseconds(13, gerakServo(170));
        delay(200);
        kit1.writeMicroseconds(14, gerakServo(70));
        break;
      case 'k':  //buka capit
        kit1.writeMicroseconds(14, gerakServo(0));
        input = 0;
        break;
      case 'l':  //tutup capit
        servo_pindah_korban();
        input = 0;
        break;
      case 'n':
        servo_park_home_1();
        input = 0;
        break;
      case 'm':
        for (int i = 0; i > -50; i = i - 10) {
          Yoff = i;  //body mundur
          delay(5);
        }
        input = 0;
        break;
      case 'o':  //capit maju
        servo_ambil_korban_1();
        input = 0;
        break;
      case 'p':  // capit mundur
        servo_ambil_korban_1_1();
        input = 0;
        break;
      case 'q':  //pivot kiri
        input = 5;
        break;
      case 's':  //mundur
        input = 3;
        break;
      case 'u':
        servo_ambil_korban_0();
        lebarLangkah = 20;
        input = 0;
        break;
      case 'w':  //maju
        input = 1;
        break;
      case 'y':
        servo_ambil_korban_0_2();
        lebarLangkah = 18;  //maju
        input = 0;
        break;
      case ' ':
        input = 50;
        break;
      default:
        input = 0;
        break;
    }
    passData(input);
  }
}