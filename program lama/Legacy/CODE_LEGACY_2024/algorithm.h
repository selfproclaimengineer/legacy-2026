float kalmanFilter(float SensorData, float R, float Q) {
  static float Xt_prev = 0;
  static float Pt_prev = 1;
  static int initialized = 0;
  float Xt_update, Pt_update, Kt, Xt, Pt;

  if (R <= 0 || Q <= 0) {
    return SensorData;  // Menghindari input tidak valid
  }

  if (!initialized) {
    Xt_prev = SensorData;
    Pt_prev = 1;
    initialized = 1;
  }

  // Prediksi
  Xt_update = Xt_prev;
  Pt_update = Pt_prev + Q;

  // Cek untuk menghindari pembagian dengan nol
  if (Pt_update + R == 0) {
    return SensorData;
  }

  // Kalkulasi gain Kalman
  Kt = Pt_update / (Pt_update + R);

  // Update estimasi
  Xt = Xt_update + Kt * (SensorData - Xt_update);
  Pt = (1 - Kt) * Pt_update;

  // Simpan untuk iterasi berikutnya
  Xt_prev = Xt;
  Pt_prev = Pt;

  return Xt;
}
