#include<Arduino.h>
#include<Wire.h>
#include<TFLI2C.h>
#include<Servo.h>

#define LIDAR_MIN_DIST 0
#define LIDAR_MAX_DIST 900 // in cm
#define SAMPLES 5

Servo servoX;
Servo servoY;

int pointArray[3];
int i;

TFLI2C tfl;
int16_t tfDist;
int16_t tfAddr = TFL_DEF_ADR;

void setup()
{
  Serial.begin( 115200 );
  Wire.begin();
  servoX.attach(9);
  servoY.attach(10);
}

void loop()
{
  
}

int LidarReading()
{
  if(tfl.getdata( tfDist, tfAddr))
  {
    return tfDist;
  }
  delay(10);
  return -1;
  
}

float SmoothLidarReading() {
  float averageDist = 0;
  for (int i = 0; i < SAMPLES; i = i + 1) {
    int x = float(LidarReading());
    while (x < LIDAR_MIN_DIST || x > LIDAR_MAX_DIST) {
      x = float(LidarReading());
    }
    averageDist = averageDist + x;
  }
  averageDist = averageDist / SAMPLES;
  return averageDist;
}

