#include<Arduino.h>
#include<Wire.h>
#include<TFLI2C.h>
#include<ESP32Servo.h>

#define LIDAR_MIN_DIST 3
#define LIDAR_MAX_DIST 900 // in cm
#define SAMPLES 5

Servo servoPhi;
Servo servoTheta;

int pointArray[3];
int i;
int thetaAngle = 180;
int phiAngle = 0;
int mapthetaAngle;
TFLI2C tfl;
int16_t tfDist;
int16_t tfAddr = TFL_DEF_ADR;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  servoPhi.setPeriodHertz(50);
  servoTheta.setPeriodHertz(50);
  servoPhi.attach(19, 500, 2500);
  servoTheta.attach(18, 500, 2500);
  servoPhi.write(phiAngle);
  servoTheta.write(thetaAngle);
}

void loop()
{
  if (thetaAngle <= 180 && thetaAngle >= 90)
  {
    for (phiAngle = 30; phiAngle <= 180; phiAngle++)
    {
      pointArray[0] = LidarReading();
      pointArray[2] = phiAngle;
      printArray();
      servoPhi.write(phiAngle);
      delay(30);
    }
    //moveUp();
    for (phiAngle = 180; phiAngle >= 30; phiAngle--)
    {
      pointArray[0] = LidarReading();
      pointArray[2] = phiAngle;
      printArray();
      servoPhi.write(phiAngle);
      delay(30);
    }
    //moveUp();
  }
  if(thetaAngle == 90 && phiAngle == 180){
    servoPhi.write(0);
    servoTheta.write(180);
    Serial.end();
  }
}

void moveUp()
{
  if (thetaAngle <= 180 && thetaAngle >= 90)
  {
    thetaAngle -= 1;
    servoTheta.write(thetaAngle);
    mapthetaAngle = map(thetaAngle, 180, 90, 0, 90);
    pointArray[1] = mapthetaAngle;
    printArray();
    delay(30);
  }
}

int LidarReading()
{
  if(tfl.getData(tfDist, tfAddr))
  {
     return tfDist;
  }
  else
  {
    return -1;
  }
  
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


void printArray() {
  for (int i = 0; i < 3; i++) {
    
    Serial.print(pointArray[i]);
    
    if (i != 2) {
      Serial.print(",");
    }
  }
  Serial.println();
}
