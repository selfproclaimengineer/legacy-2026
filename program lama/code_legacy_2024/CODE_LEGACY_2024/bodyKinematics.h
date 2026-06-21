#include <SpeedTrig.h>

// Struktur untuk menyimpan posisi awal dan rotasi dari setiap bagian tubuh hexapod
struct body {
    float awalX, awalY, awalZ;  // Posisi awal (koordinat X, Y, Z)
    float posX, posY, posZ;     // Posisi saat ini setelah rotasi dan translasi
} body[6];

// Konstanta untuk ukuran tubuh hexapod
#define panjangBody 78
#define lebarBody 90
#define lebarKecilBody 45

// Fungsi untuk menginisialisasi posisi awal tubuh hexapod
void setupBody() {
    body[0].awalX = lebarKecilBody;
    body[1].awalX = lebarBody;
    body[2].awalX = lebarKecilBody;
    body[3].awalX = (-1) * lebarKecilBody;
    body[4].awalX = (-1) * lebarBody;
    body[5].awalX = (-1) * lebarKecilBody;

    body[0].awalY = panjangBody;
    body[1].awalY = 0;
    body[2].awalY = (-1) * panjangBody;
    body[3].awalY = (-1) * panjangBody;
    body[4].awalY = 0;
    body[5].awalY = panjangBody;

    for (int i = 0; i < 6; i++) {
        body[i].awalZ = 0;  // Set posisi awal Z ke 0
    }
}

// Matriks rotasi untuk sumbu X (roll)
void rotasiX(float angle, float matrix[3][3]) {
    float cosA = SpeedTrig.cos(angle);
    float sinA = SpeedTrig.sin(angle);
    matrix[0][0] = 1; matrix[0][1] = 0;    matrix[0][2] = 0;
    matrix[1][0] = 0; matrix[1][1] = cosA; matrix[1][2] = -sinA;
    matrix[2][0] = 0; matrix[2][1] = sinA; matrix[2][2] = cosA;
}

// Matriks rotasi untuk sumbu Y (pitch)
void rotasiY(float angle, float matrix[3][3]) {
    float cosA = SpeedTrig.cos(angle);
    float sinA = SpeedTrig.sin(angle);
    matrix[0][0] = cosA; matrix[0][1] = 0; matrix[0][2] = sinA;
    matrix[1][0] = 0;    matrix[1][1] = 1; matrix[1][2] = 0;
    matrix[2][0] = -sinA; matrix[2][1] = 0; matrix[2][2] = cosA;
}

// Matriks rotasi untuk sumbu Z (yaw)
void rotasiZ(float angle, float matrix[3][3]) {
    float cosA = SpeedTrig.cos(angle);
    float sinA = SpeedTrig.sin(angle);
    matrix[0][0] = cosA; matrix[0][1] = -sinA; matrix[0][2] = 0;
    matrix[1][0] = sinA; matrix[1][1] = cosA;  matrix[1][2] = 0;
    matrix[2][0] = 0;    matrix[2][1] = 0;     matrix[2][2] = 1;
}

// Fungsi untuk mengalikan matriks rotasi dengan vektor posisi (X, Y, Z)
void multiplyMatrixVector(float matrix[3][3], float x, float y, float z, float &newX, float &newY, float &newZ) {
    newX = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
    newY = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
    newZ = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
}

// Fungsi untuk menerapkan semua rotasi (roll, pitch, yaw) ke tubuh
void applyBodyRotation(float rollAngle, float pitchAngle, float yawAngle) {
    float rotX[3][3], rotY[3][3], rotZ[3][3];
    
    // Dapatkan matriks rotasi untuk setiap sumbu
    rotasiX(rollAngle, rotX);
    rotasiY(pitchAngle, rotY);
    rotasiZ(yawAngle, rotZ);

    // Aplikasikan rotasi ke setiap kaki
    for (int i = 0; i < 6; i++) {
        float tempX, tempY, tempZ;
        
        // Pertama, rotasi dengan matriks roll (sumbu X)
        multiplyMatrixVector(rotX, body[i].awalX, body[i].awalY, body[i].awalZ, tempX, tempY, tempZ);
        
        // Kedua, rotasi dengan matriks pitch (sumbu Y)
        multiplyMatrixVector(rotY, tempX, tempY, tempZ, tempX, tempY, tempZ);
        
        // Terakhir, rotasi dengan matriks yaw (sumbu Z)
        multiplyMatrixVector(rotZ, tempX, tempY, tempZ, body[i].posX, body[i].posY, body[i].posZ);
    }
}

//sudah dibetulkan dengan perkalian matriks 3x3 namun belum dicoba untuk mengimplementasikannya