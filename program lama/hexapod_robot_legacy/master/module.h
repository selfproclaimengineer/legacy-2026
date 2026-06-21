#include <Wire.h>
#include <MechaQMC5883.h>
#define srf0 cekJarak(40)
#define srf1 cekJarak(46)
#define srf2 cekJarak(47)
#define srf3 cekJarak(44)
#define srf5 cekJarak(42)  //salah pin
MechaQMC5883 qmc;
#define CMPS11_ADDRESS 0x0D
int degree = 0;
#define lengan 95
#define gripper 95
#define grip 40

void setupKompas() {
  Wire.begin();
  qmc.init();
}

float gerakServo1(float nilai) {
  return map(nilai, 0, 180, 500, 2550);
}

/*
void park_2() {
  kit0.writeMicroseconds(5, gerakServo(50));
  //delay(400);
  kit0.writeMicroseconds(6, gerakServo(100));
  delay(200);
  kit0.writeMicroseconds(4, gerakServo(15));
  //15 -- buka
  //70 -- tutup
}

void park_1() {
  kit0.writeMicroseconds(4, gerakServo(70));
  delay(100);
  kit0.writeMicroseconds(5, gerakServo(180));
  kit0.writeMicroseconds(6, gerakServo(130));
  
//  delay(500);
  
  //15 -- buka
  //70 -- tutup
}

void park_3() {
  
 kit0.writeMicroseconds(5, gerakServo(50));
  //delay(400);
  kit0.writeMicroseconds(6, gerakServo(100));
  //delay(400);
  kit0.writeMicroseconds(4, gerakServo(15));
  
  //15 -- buka
  //70 -- tutup
}*/
/*
void servo_park_home(){
   kit1.writeMicroseconds(4, gerakServo1(0)); //body
  kit1.writeMicroseconds(5, gerakServo1(75)); //tengah
  kit1.writeMicroseconds(6, gerakServo1(0)); //capit

  kit0.writeMicroseconds(5, gerakServo(180)); // body
  kit0.writeMicroseconds(6, gerakServo(155)); // tengah
  kit0.writeMicroseconds(4, gerakServo1(0));
  
}


void servo_ambil_korban_0(){
  kit0.writeMicroseconds(4, gerakServo1(0));
  kit0.writeMicroseconds(5, gerakServo(60)); // body
  kit0.writeMicroseconds(6, gerakServo(60)); // tengah

 
}

void servo_ambil_korban_1_0(){
  kit0.writeMicroseconds(4, gerakServo1(50)); //capit
  delay(500);
  kit0.writeMicroseconds(5, gerakServo(180)); // body
  delay(200);
  kit0.writeMicroseconds(6, gerakServo(175)); // tengah

  
}

void servo_ambil_korban_1_1(){
  kit1.writeMicroseconds(6, gerakServo1(50)); //capit
  kit1.writeMicroseconds(4, gerakServo1(0)); //body
  kit1.writeMicroseconds(5, gerakServo1(75)); //tengah
}
void servo_pindah_korban(){
  kit0.writeMicroseconds(4, gerakServo1(10)); //capit
}

void servo_park_home_1(){
  kit1.writeMicroseconds(6, gerakServo1(50)); //capit
  /*kit1.writeMicroseconds(4, gerakServo1(0)); //body
  kit1.writeMicroseconds(5, gerakServo1(75)); //tengah
  kit0.writeMicroseconds(5, gerakServo(180)); // body
  kit0.writeMicroseconds(6, gerakServo(175)); // tengah
  kit0.writeMicroseconds(4, gerakServo1(50)); //capit 
}*/
/*
void servo_pindah_korban_1(){
  //kit1.writeMicroseconds(6, gerakServo1(50)); //capit
  kit0.writeMicroseconds(4, gerakServo1(0)); // capit
  delay(100);
  kit0.writeMicroseconds(6, gerakServo(140)); // tengah
  kit0.writeMicroseconds(5, gerakServo(140)); // body
 // kit1.writeMicroseconds(5, gerakServo1(70)); //tengah
  //kit1.writeMicroseconds(4, gerakServo1(0)); //body 
}

void servo_taruh_korban(){
  kit1.writeMicroseconds(4, gerakServo1(140)); //body
  kit1.writeMicroseconds(5, gerakServo1(110)); //tengah
  delay(500);
  kit1.writeMicroseconds(6, gerakServo1(0)); //capit
}*/



void servo_park_home(){
  // Park home untuk robot 0
  kit0.writeMicroseconds(12, gerakServo(75)); // body 75
  kit0.writeMicroseconds(13, gerakServo(160)); // tengah
  kit0.writeMicroseconds(14, gerakServo1(0)); // capit 


  // Park home untuk robot 1
  kit1.writeMicroseconds(12, gerakServo1(100)); // body
  kit1.writeMicroseconds(13, gerakServo1(80)); // tengah
  kit1.writeMicroseconds(14, gerakServo1(0)); // capit
}

void servo_park_home_1(){
  // Park home untuk robot 0
  kit0.writeMicroseconds(12, gerakServo(150)); // body
  kit0.writeMicroseconds(13, gerakServo(170)); // tengah
  kit0.writeMicroseconds(14, gerakServo1(50)); // capit 
}
void servo_ambil_korban_0(){
  // Ambil korban untuk robot 0
  kit1.writeMicroseconds(12, gerakServo1(0));
  kit1.writeMicroseconds(13, gerakServo(175));
  delay(200); 
  kit1.writeMicroseconds(14, gerakServo(0)); 
}

void servo_ambil_korban_0_2(){
  // Ambil korban untuk robot 0
  kit1.writeMicroseconds(12, gerakServo1(5));
  kit1.writeMicroseconds(13, gerakServo(180));
  delay(200); 
  kit1.writeMicroseconds(14, gerakServo(0)); 
}

void servo_ambil_korban_0_1(){
  // Ambil korban untuk robot 1
  kit1.writeMicroseconds(14, gerakServo1(70)); // capit
  delay(300);
  kit1.writeMicroseconds(13, gerakServo(145)); // tengah
  delay(100);
  kit1.writeMicroseconds(12, gerakServo(180)); // body
}

void servo_ambil_korban_1(){
  // Ambil korban untuk robot 1
  
  kit0.writeMicroseconds(12, gerakServo1(180)); // body
  delay(100);
  kit0.writeMicroseconds(13, gerakServo1(75)); // tengah
  delay(400);
  kit0.writeMicroseconds(14, gerakServo1(0)); // capit
}

void servo_ambil_korban_1_1(){
  // Ambil korban untuk robot 1
  kit0.writeMicroseconds(14, gerakServo1(50)); // capit
  delay(200);
  kit0.writeMicroseconds(13, gerakServo(170)); // tengah
  delay(200);
  kit0.writeMicroseconds(12, gerakServo(50)); // body
  //delay(100);
  
}

void servo_pindah_korban(){
  // Pindah korban
  kit0.writeMicroseconds(14, gerakServo1(50)); // capit
  delay(200);
  kit1.writeMicroseconds(14, gerakServo(0)); //
}
void inverseCapit(float y, float z) {
  float l, l2, l3, sudut1, sudut2, temp3;
  float lengan_2, grip_2;
  float y1, z1;

  y1 = y - gripper;
  z1 = z + grip;
  lengan_2 = lengan * lengan;
  grip_2 = grip * grip;
  l2 = sqrt((y1 * y1) + (z * z));
  l3 = l2 * l2;
  sudut1 = SpeedTrig.atan2(z1, y1);

  temp3 = (((lengan_2 + grip_2) - l3) / ((2 * lengan) * grip));
  sudut2 = SpeedTrig.acos(temp3);

  sudut1 = float2int(rad2deg(sudut1));
  sudut2 = float2int(rad2deg(sudut2));

  sudut1 = sudut1 + 90;
  sudut2 = sudut2 - 90;
  // Serial.print(sudut1);
  // Serial.print(", ");
  // Serial.println(sudut2);
  kit0.writeMicroseconds(5, gerakServo(sudut1));
  kit0.writeMicroseconds(6, gerakServo(sudut2));
}

int bacaKompas() {
  int x, y, z;
  int degree;
  qmc.read(&x, &y, &z, &degree);
  // Serial.println(degree);
  return degree;
}

int cekJarak(int srf) {
  long duration, distance;
  pinMode(srf, OUTPUT);
  digitalWrite(srf, LOW);
  delayMicroseconds(2);
  digitalWrite(srf, HIGH);
  delayMicroseconds(5);
  digitalWrite(srf, LOW);
  pinMode(srf, INPUT);
  duration = pulseIn(srf, HIGH);
  distance = duration / 29 / 2;
  return distance;
}