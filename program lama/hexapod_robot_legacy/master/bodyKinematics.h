#include <SpeedTrig.h>
struct body {
  float awalX, awalY, awalZ;
  float xRotX, yRotX, zRotX; 
  float xRotY, yRotY, zRotY; 
  float xRotZ, yRotZ, zRotZ; 
  float totX, totY, totZ;
} body[6];

#define panjangBody 78
#define lebarBody 90
#define lebarKecilBody 45

void setupBody() {
  body[0].awalX = lebarKecilBody;
  body[1].awalX = lebarBody;
  body[2].awalX = lebarKecilBody;
  body[3].awalX = (-1) * lebarKecilBody;
  body[4].awalX = (-1) * lebarBody;
  body[5].awalX = (-1) * lebarKecilBody;

  // potition y
  body[0].awalY = panjangBody;
  body[1].awalY = 0;
  body[2].awalY = (-1) * panjangBody;
  body[3].awalY = (-1) * panjangBody;
  body[4].awalY = 0;
  body[5].awalY = panjangBody;

  // potition X
  for (int i = 0; i < 6; i++) {
    body[i].awalZ = 0;
  }
}

void roll_x(float angle) {
  float y2, y3, z2, z3;
  
  for (int i = 0; i < 6; i++) {
    y2 = body[i].awalY * SpeedTrig.cos(angle);
    y3 = body[i].awalZ * SpeedTrig.sin(angle);
    z2 = body[i].awalZ * SpeedTrig.cos(angle);
    z3 = body[i].awalY * SpeedTrig.sin(angle);

    body[i].yRotX = y2 - y3;
    body[i].zRotX = z2 + z3;
    body[i].xRotX = body[i].awalX;

    body[i].yRotX = body[i].yRotX - body[i].awalY;
    body[i].zRotX = body[i].zRotX - body[i].awalZ;
    body[i].xRotX = body[i].xRotX - body[i].awalX;

    if( i == 3){
      // Serial.print(body[i].xRotX);
      // Serial.print(", ");
      // Serial.print(body[i].yRotX);
      // Serial.print(", ");
      // Serial.println(body[i].zRotX);
    }
  }
}

void pitch_y(float angle) {
  float x2, x3, z2, z3;
  
  for (int i = 0; i < 6; i++) {
    x2 = body[i].awalX * SpeedTrig.cos(angle);
    x3 = body[i].awalZ * SpeedTrig.sin(angle);
    z2 = body[i].awalZ * SpeedTrig.cos(angle);
    z3 = body[i].awalX * SpeedTrig.sin(angle);

    body[i].xRotY = x2 + x3;
    body[i].zRotY = z2 - z3;
    body[i].yRotY = body[i].awalY;

    body[i].yRotY = body[i].yRotY - body[i].awalY;
    body[i].zRotY = body[i].zRotY - body[i].awalZ;
    body[i].xRotY = body[i].xRotY - body[i].awalX;

    if( i == 3){
      // Serial.print(body[i].xRotY);
      // Serial.print(", ");
      // Serial.print(body[i].yRotY);
      // Serial.print(", ");
      // Serial.println(body[i].zRotY);
    }
  }
}

void yaw_z(float angle) {
  float x2, x3, y2, y3;   
  for (int i = 0; i < 6; i++) {
    x2 = body[i].awalX * SpeedTrig.cos(angle);
    x3 = body[i].awalY * SpeedTrig.sin(angle);
    y2 = body[i].awalY * SpeedTrig.cos(angle);
    y3 = body[i].awalX * SpeedTrig.sin(angle);

    body[i].xRotZ = x2 - x3;
    body[i].yRotZ = y2 + y3;
    body[i].zRotZ = body[i].awalZ;

    body[i].yRotZ = body[i].yRotZ - body[i].awalY;
    body[i].zRotZ = body[i].zRotZ - body[i].awalZ;
    body[i].xRotZ = body[i].xRotZ - body[i].awalX;

    // if( i == 3){
    //   Serial.print(body[i].xRotY);
    //   Serial.print(", ");
    //   Serial.print(body[i].yRotY);
    //   Serial.print(", ");
    //   Serial.println(body[i].zRotY);
    // }
  }
}