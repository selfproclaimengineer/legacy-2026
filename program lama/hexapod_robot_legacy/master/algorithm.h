int sudutHadap = 0;
#define utara 105
#define timur 200
#define selatan 270
#define barat 345

void mulaiAwal() {
  sudutHadap = bacaKompas();
  int toleransi = 20;
  Serial.println("maju");
  while (!((sudutHadap > (selatan - toleransi)) && (sudutHadap < (selatan + toleransi)))) {
    sudutHadap = bacaKompas();
    if ((sudutHadap <= utara && sudutHadap >= 0) || (sudutHadap >= selatan + toleransi && sudutHadap <= 360)) {
      passData(5);  //pivot kiri
      Serial.println("lebih kanan");
    } else if (sudutHadap <= selatan - toleransi && sudutHadap > utara) {
      passData(6);  //pivot kanan
      Serial.println("lebih kiri");
    }
  }
}