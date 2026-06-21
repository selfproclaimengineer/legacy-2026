// Uji mandiri IK di PC (tanpa hardware).
//   g++ -I.. test_kinematics.cpp ../LegIK.cpp -o t && ./t
// Strategi: IK(target) -> sudut -> FK(sudut) harus kembali ke target.
#include "../LegIK.h"
#include "../types.h"
#include <cstdio>
#include <cassert>

// Forward Kinematics independen untuk verifikasi.
static Vec3 fk(float coxaDeg, float femurDeg, float tibiaDeg) {
    float c = deg2rad(coxaDeg), f = deg2rad(femurDeg), k = deg2rad(tibiaDeg);
    // bidang vertikal kaki: u = horizontal sepanjang arah coxa, v = z
    float kneeU = COXA_LENGTH + FEMUR_LENGTH * cosf(f);
    float kneeV = FEMUR_LENGTH * sinf(f);
    float tibiaDir = f - ((float)M_PI - k);   // arah segmen tibia
    float footU = kneeU + TIBIA_LENGTH * cosf(tibiaDir);
    float footV = kneeV + TIBIA_LENGTH * sinf(tibiaDir);
    return { footU * cosf(c), footU * sinf(c), footV };
}

static int check(float x, float y, float z) {
    float co, fe, ti;
    bool ok = LegIK::solve(x, y, z, co, fe, ti);
    Vec3 p = fk(co, fe, ti);
    float ex = fabsf(p.x - x), ey = fabsf(p.y - y), ez = fabsf(p.z - z);
    bool pass = ok && ex < 0.5f && ey < 0.5f && ez < 0.5f;
    printf("target(%6.1f,%6.1f,%6.1f) -> coxa=%6.1f femur=%6.1f tibia=%6.1f | err(%.2f,%.2f,%.2f) %s\n",
           x, y, z, co, fe, ti, ex, ey, ez, pass ? "OK" : (ok ? "FK-MISMATCH" : "OUT-OF-RANGE"));
    return pass ? 0 : 1;
}

int main() {
    int fails = 0;
    // Beberapa pose realistis (z negatif = kaki di bawah badan).
    fails += check(STAND_RADIUS, 0, -STAND_HEIGHT);
    fails += check(70, 0, -90);
    fails += check(60, 20, -100);
    fails += check(90, 0, -60);
    fails += check(50, -10, -110);
    fails += check(80, 30, -80);

    // Di luar jangkauan harus ditandai (bukan crash/NaN).
    float a,b,c;
    bool ok = LegIK::solve(500, 0, 0, a, b, c);
    printf("over-reach flagged out-of-range: %s\n", !ok ? "OK" : "FAIL");
    if (ok) fails++;

    printf(fails ? "\n[FAIL] %d kasus gagal\n" : "\n[PASS] semua kasus IK konsisten\n", fails);
    return fails ? 1 : 0;
}
