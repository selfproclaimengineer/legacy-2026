#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void tampilkanData(String label, float value) {
  display.clearDisplay();  // Bersihkan layar OLED
  display.setTextSize(1);  // Set ukuran teks
  display.setTextColor(SSD1306_WHITE);  // Set warna teks

  // Tampilkan label data
  display.setCursor(0, 0);
  display.print(label);  // Tampilkan label (misalnya: "Suhu")

  // Tampilkan nilai data
  display.setCursor(0, 16);
  display.print("Value: ");
  display.print(value, 2);  // Tampilkan nilai variabel dengan 2 angka di belakang koma

  display.display();  // Perbarui tampilan OLED
}

// Fungsi untuk menampilkan nilai kompas di layar OLED (DIPINDAHKAN KE FILE .INO)
void tampilkanKompas(int yaw) {
  display.clearDisplay();  // Bersihkan layar OLED
  display.setTextSize(1);  // Set ukuran teks
  display.setTextColor(SSD1306_WHITE);  // Set warna teks

  // Tampilkan judul "Kompas"
  display.setCursor(0, 0);
  display.println(F("Kompas CMP10A"));

  // Tampilkan nilai sudut Z (Yaw)
  display.setCursor(0, 16);
  display.print(F("Yaw (Z): "));
  display.print(yaw);  // Tampilkan nilai yaw

  display.display();  // Perbarui tampilan OLED
}