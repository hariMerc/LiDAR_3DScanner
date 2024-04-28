#include<Servo.h>
#include<SoftwareSerial.h>
#include "TFMini.h"

#define LIDAR_MIN_DIST 20
#define LIDAR_MAX_DIST 800 // in cm
#define SAMPLES 5


Servo servoAlpha; //Vertial servo
Servo servoTheta; //Horizontal servo

SoftwareSerial mySerial(10, 11); //Defines the TFMini TX pin and TFMini RX pin
TFMini tfmini; //Object for the LiDAR sensor

uint16_t dist; //holds the distance(in cm)
int i;
int pointArray[3]; //Holds the 0. hAngle 1. vAngle 2. distance

int angle; 
int verAngle; 
int verAngleStop;
int angleFrom; 
int angleTo;
int mapAngle;


void setup()
{
	Serial.begin(115200);
	Serial3.begin(115200);
	mySerial.begin(TFMINI_BAUDRATE);
	tfmini.begin(&mySerial);
	servoAlpha.attach(9); // attaches the vertical servo on pin 9 to the servo object
	servoTheta.attach(10); // attaches the horizontal servo on pin 10 to the servo object
	
	//Horizontal Servo motion on startup
	servoTheta.writeMicroseconds(2000);//left
	delay(950);
	servoTheta.writeMicroseconds(1000);//right
	delay(950);
	servoTheta.writeMicroseconds(1500);//middle
	delay(950);
	//Vertical servo motion on startup
	servoAlpha.writeMicroseconds(1500);
	delay(950);
	servoAlpha.writeMicroseconds(1700);
	delay(950);
	servoAlpha.writeMicroseconds(1000);
	delay(950);
	
	
}

void loop()
{
	
	if (verAngle >= verAngleStop) 
	{
		for (angle = angleFrom; angle <= angleTo; angle += 7)
		{
		  pointArray[2] = SmoothLidarReading();
		  mapAngle = angle;
		  mapAngle = map(mapAngle, 500, 2500, 0, 180);
		  printArray();
		  pointArray[0] = mapAngle;
		  servo1.writeMicroseconds(angle);
		  delay(horizonDelay);
		}
		MoveUp();
		// And back from 2500 to 0 degrees:
		for (angle = angleTo; angle >= angleFrom; angle -= 7)
		{
		  pointArray[2] = SmoothLidarReading();
		  mapAngle = angle;
		  mapAngle = map(mapAngle, 500, 2500, 0, 180);
		  printArray();
		  pointArray[0] = mapAngle;
		  servo1.writeMicroseconds(angle);
		  delay(horizonDelay);
		}
		MoveUp();
    }
}

void MoveUp() 
{
  if (verAngle >= verAngleStop) 
  {
    verAngle = verAngle - 7;
    servo2.writeMicroseconds(verAngle);
    mapVerAngle = verAngle;
    mapVerAngle = map(mapVerAngle, 500, 2500, 180, -90);
    pointArray[1] = mapVerAngle;
    delay(20);
  }
}

int LidarReading()
{
	dist = tfmini.getDistance();
	delay(10);
	return dist;
}

float SmoothLidarReading()
{
	float averageDist = 0;
	for (int i = 0; i < SAMPLES; i = i + 1) 
	{
		int x = float(LidarReading());
		while (x < LIDAR_MIN_DIST || x > LIDAR_MAX_DIST) 
		{
		  x = float(LidarReading());
		}
		averageDist = averageDist + x;
	}
  averageDist = averageDist / SAMPLES;
  return averageDist;
}

void printArray() 
{
  for (int i = 0; i < 3; i++)
  {
    Serial.print(pointArray[i]);
    if (i != 2) 
    {
      Serial.print(" ");
    }
  }
  Serial.println();
}
