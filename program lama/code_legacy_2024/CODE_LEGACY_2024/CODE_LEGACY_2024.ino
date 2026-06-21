#include "controlSystem.h"
#include "oled.h"
#include "bodyKinematics.h"
#include "kaki.h"
#include "kompas.h"
#include "komponen.h"
#include "lidar.h"
#include "algorithm.h"
#include "kiri.h"
// #include "prog.h"

bool bag = 0;

#define pb 30

int j, k, l, m, n, o, p;
char keyInput;



void setup() {
  Serial.begin(9600);
  pinMode(pb, INPUT_PULLUP);
  SERIAL_PORT.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.display();
  display.clearDisplay();
  
  setupSensor();
  setupKaki();
  setupBody();
  servo_park_home();
  passData(0);
  Serial.println("Halo");
}

void loop() {
  tampilkanData("Kompas", bacaKompas());

  Serial.println(lid0);
  if (digitalRead(pb) == LOW) {
    // tampilkanData("Kompas", bacaKompas());

    menujuKorban1();
    ambilKorban1();
    menujuSz1();
    // ikutBarat(5);
    // menujuKorban1();
    // ikutSelatan(5);
  }
}
