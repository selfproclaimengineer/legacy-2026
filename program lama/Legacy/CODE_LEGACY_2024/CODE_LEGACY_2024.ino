#include "algorithm.h"
#include "lidar.h"
#include "oled.h"
#include "bodyKinematics.h"
#include "kaki.h"
#include "kompas.h"
#include "komponen.h"
#include "ikutDinding.h"
#include "kiri.h"

bool bag = 0;

#define pb 30
#define pb1 31
#define pb2 29

int j, k, l, m, n, o, p;
char keyInput;
int Yoff = 0;
int Xoff = 0;

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
      servo_ambil_korban_1();
  servo_ambil_korban_1_1();
}

void loop() {

  // bacaLidar();
    // Jika push button ditekan, robot akan melakukan translasi
    if (digitalRead(pb) == 0) {
    menujuKorban1();
    ambilKorban1();
    // ikutBarat();
    // menujuSz1();
    // ikutSelatan();
    // menujuKorban2();
    // menujuSz2();
    // kontol();
    // menujuLantai2();
    // ikutBarat();
    // ambilKorban2();
    // menujuSz3();
    // ikutSelatan();
    // menujuTangga();
    // naikTangga();
    // ikutTimur();
}
}

