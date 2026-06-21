// informasi fuzzylogic dapat dilihat disini
// https://www.anakkendali.com/2020/06/07/tutorial-aturan-fuzzy-logic-controller-dengan-arduino/
#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H

float sudut, langkahKiri, langkahKanan, outKanan, outKiri;  //input
float sudutF[3];
#define utara 5//335 // Definisi arah utara
#define timur 260 // Definisi arah timur
#define selatan 175//185 // Definisi arah selatan
#define barat 70//115 // Definisi arah barat
int knop = 0;
int toler = 0;

float kalmanFilter(float SensorData, float R, float Q) {
  static float Xt_prev = 0;    // Status sebelumnya
  static float Pt_prev = 1;    // Error covarian sebelumnya
  float Xt_update, Pt_update, Kt, Xt, Pt;
  float KalmanFilterData;

  // Prediksi
  Xt_update = Xt_prev;         // Update prediksi status
  Pt_update = Pt_prev + Q;     // Update prediksi error covarian

  // Kalkulasi gain Kalman
  Kt = Pt_update / (Pt_update + R);

  // Update estimasi
  Xt = Xt_update + Kt * (SensorData - Xt_update);
  Pt = (1 - Kt) * Pt_update;

  // Simpan nilai status dan error covarian untuk iterasi berikutnya
  Xt_prev = Xt;
  Pt_prev = Pt;

  // Simpan hasil ke KalmanFilterData dan return
  KalmanFilterData = Xt;

  return KalmanFilterData;  // Kembalikan hasil filter Kalman
}

void fuzzySudut(int batasBawah, int batasTengah, int batasAtas) {
  if (sudut <= batasBawah) sudutF[0] = 1;
  else if ((sudut >= batasBawah) && (sudut <= batasTengah)) {
    sudutF[0] = ((batasTengah - sudut) / (batasTengah - batasBawah));
  } else if (sudut >= batasTengah) sudutF[0] = 0;

  if (sudut <= batasBawah) sudutF[1] = 0;
  else if ((sudut >= batasBawah) && (sudut <= batasTengah)) {
    sudutF[1] = ((sudut - batasBawah) / (batasTengah - batasBawah));
  } else if ((sudut >= batasTengah) && (sudut <= batasAtas)) {
    sudutF[1] = ((batasAtas - sudut) / (batasTengah - batasBawah));
  } else if (sudut >= batasAtas) sudutF[1] = 0;

  if (sudut <= batasTengah) sudutF[2] = 0;
  else if ((sudut >= batasTengah) && (sudut <= batasAtas)) {
    sudutF[2] = ((sudut - batasTengah) / (batasTengah - batasBawah));
  } else if (sudut >= batasAtas) sudutF[2] = 1;
}


void fuzzyLangkahKanan(int batasBawah, int batasTengah, int batasAtas) {
  float langkahKananF[3];
  if (langkahKanan <= batasBawah) langkahKananF[0] = 1;
  else if ((langkahKanan >= batasBawah) && (langkahKanan <= batasTengah)) {
    langkahKananF[0] = ((batasTengah - langkahKanan) / (batasTengah - batasBawah));
  } else if (langkahKanan >= batasTengah) langkahKananF[0] = 0;

  if (langkahKanan <= batasBawah) langkahKananF[1] = 0;
  else if ((langkahKanan >= batasBawah) && (langkahKanan <= batasTengah)) {
    langkahKananF[1] = ((langkahKanan - batasBawah) / (batasTengah - batasBawah));
  } else if ((langkahKanan >= batasTengah) && (langkahKanan <= batasAtas)) {
    langkahKananF[1] = ((batasAtas - langkahKanan) / (batasTengah - batasBawah));
  } else if (langkahKanan >= batasAtas) langkahKananF[1] = 0;

  if (langkahKanan <= batasTengah) langkahKananF[2] = 0;
  else if ((langkahKanan >= batasTengah) && (langkahKanan <= batasAtas)) {
    langkahKananF[2] = ((langkahKanan - batasTengah) / (batasTengah - batasBawah));
  } else if (langkahKanan >= batasAtas) langkahKananF[2] = 1;
}

void fuzzyLangkahKiri(int batasBawah, int batasTengah, int batasAtas) {
  float langkahKiriF[3];
  if (langkahKiri <= batasBawah) langkahKiriF[0] = 1;
  else if ((langkahKiri >= batasBawah) && (langkahKiri <= batasTengah)) {
    langkahKiriF[0] = ((batasTengah - langkahKiri) / (batasTengah - batasBawah));
  } else if (langkahKiri >= batasTengah) langkahKiriF[0] = 0;

  if (langkahKiri <= batasBawah) langkahKiriF[1] = 0;
  else if ((langkahKiri >= batasBawah) && (langkahKiri <= batasTengah)) {
    langkahKiriF[1] = ((langkahKiri - batasBawah) / (batasTengah - batasBawah));
  } else if ((langkahKiri >= batasTengah) && (langkahKiri <= batasAtas)) {
    langkahKiriF[1] = ((batasAtas - langkahKiri) / (batasTengah - batasBawah));
  } else if (langkahKiri >= batasAtas) langkahKiriF[1] = 0;

  if (langkahKiri <= batasTengah) langkahKiriF[2] = 0;
  else if ((langkahKiri >= batasTengah) && (langkahKiri <= batasAtas)) {
    langkahKiriF[2] = ((langkahKiri - batasTengah) / (batasTengah - batasBawah));
  } else if (langkahKiri >= batasAtas) langkahKiriF[2] = 1;
}

void fuzzyLogic(float follow, float setKanan, float setKiri) {
  //ubah toleransi untuk mendapatkan hasil yang akurat
  if (follow == 0) follow = utara;
  else if (follow == 1) follow = timur;
  else if (follow == 2) follow = selatan;
  else if (follow == 3) follow = barat;
  float toleransiSudut = toler;  //disamakan dengan toleransi followutara/timur/barat/selatan
  float toleransiKanan = 0.4;
  float toleransiKiri = toleransiKanan;
  float ruleKanan[4], ruleKiri[4];
  float batasSudut[3] = { (follow - toleransiSudut), follow, (follow + toleransiSudut) };
  float batasKanan[3] = { (setKanan + toleransiKanan), setKanan, (setKanan - toleransiKanan) };
  float batasKiri[3] = { (setKiri - toleransiKiri), setKiri, (setKiri + toleransiKiri) };
  //fuzzyfikasi
  fuzzySudut(batasSudut[0], batasSudut[1], batasSudut[2]);
  fuzzyLangkahKanan(batasKanan[0], batasKanan[1], batasKanan[2]);
  fuzzyLangkahKiri(batasKiri[0], batasKiri[1], batasKiri[2]);
  //implikasi kanan
  ruleKanan[0] = batasKanan[1] - (sudutF[0] * (batasKanan[1] - batasKanan[0]));
  ruleKanan[1] = batasKanan[0] + (sudutF[1] * (batasKanan[1] - batasKanan[0]));
  ruleKanan[2] = batasKanan[2] - (sudutF[1] * (batasKanan[1] - batasKanan[0]));
  ruleKanan[3] = batasKanan[1] + (sudutF[2] * (batasKanan[1] - batasKanan[0]));
  outKanan = ((ruleKanan[0] * sudutF[0]) + (ruleKanan[1] * sudutF[1]) + 
  (ruleKanan[2] * sudutF[1]) + (ruleKanan[3] * sudutF[2])) / (sudutF[0] + sudutF[1] + sudutF[1] + sudutF[2]);
  // Serial.println(outKanan);
  //implikasi kiri
  ruleKiri[0] = batasKiri[1] - (sudutF[0] * (batasKiri[1] - batasKiri[0]));
  ruleKiri[1] = batasKiri[0] + (sudutF[1] * (batasKiri[1] - batasKiri[0]));
  ruleKiri[2] = batasKiri[2] - (sudutF[1] * (batasKiri[1] - batasKiri[0]));
  ruleKiri[3] = batasKiri[1] + (sudutF[2] * (batasKiri[1] - batasKiri[0]));
  outKiri = ((ruleKiri[0] * sudutF[0]) + (ruleKiri[1] * sudutF[1]) + 
  (ruleKiri[2] * sudutF[1]) + (ruleKiri[3] * sudutF[2])) / (sudutF[0] + sudutF[1] + sudutF[1] + sudutF[2]);
  // Serial.println(outKiri);
}

#endif