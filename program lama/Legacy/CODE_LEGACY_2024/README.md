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
}

void loop() {
    // Jika push button ditekan, robot akan melakukan translasi
    if (digitalRead(pb) == 0) {
        Serial.println("Button pressed: Menggerakkan dengan translasi Xoff");
        
        // Translasi ke arah kanan dengan meningkatkan Xoff
        for (int i = 0; i <= 20; i++) {
            Xoff = i;  // Ubah nilai Xoff
            setTranslation(Xoff, Yoff);
            bodyKinematics(0.0, 0.0, 0.0);
            syncLeg();  // Sinkronkan gerakan kaki
            Serial.print("Xoff: "); Serial.println(Xoff);
            delay(100);
        }

        // Kembali ke posisi awal
        for (int i = 20; i >= 0; i--) {
            Xoff = i;
            setTranslation(Xoff, Yoff);
            bodyKinematics(0.0, 0.0, 0.0);
            syncLeg();
            Serial.print("Xoff: "); Serial.println(Xoff);
            delay(100);
        }

        // Reset nilai Xoff dan Yoff setelah selesai
        Xoff = 0;
        Yoff = 0;
        setTranslation(Xoff, Yoff);
        bodyKinematics(0.0, 0.0, 0.0);
        syncLeg();
        Serial.println("Translasi selesai, posisi kembali ke awal");
    }

    // Bagian kode yang di-comment tidak dihapus
    // Yoff -=20;
    // ikutSelatan();
    // tampilkanData("kompas", bacaKompas());
    // menujuKorban1();
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
}

void loop() {
    // Jika push button ditekan, robot akan melakukan translasi
    if (digitalRead(pb) == 0) {
    menujuKorban1();
    ikutBarat();
    menujuSz1();
    ikutSelatan();
    menujuKorban2();
    menujuSz2();
    kontol();
    menujuLantai2();
    ikutBarat();
    ambilKorban2();
    menujuSz3();
    ikutSelatan();
    menujuTangga();
    naikTangga();
}
}
    // Bagian kode yang di-comment tidak dihapus
    // Yoff -=20;
    // ikutSelatan();
    // tampilkanData("kompas", bacaKompas());
    // menujuKorban1();
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
