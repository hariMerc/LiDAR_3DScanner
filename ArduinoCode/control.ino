#include<Servo.h>
#include<SoftwareSerial.h>
#include "TFMini.h"

#define LIDAR_MIN_DIST 0
#define LIDAR_MAX_DIST 800 // in cm
#define SAMPLES 5


Servo servoAlpha; //Vertial servo
Servo servoTheta; //Horizontal servo

SoftwareSerial mySerial(10, 11); //Defines the TFMini TX pin and TFMini RX pin
TFMini tfmini; //Object for the LiDAR sensor

int dist; //holds the distance(in cm)
int i;


void setup()
{
	Serial.begin(115200);
	servoAlpha.attach(9);
	servoTheta.attach(10);
}

int LidarReading()
{
	dist = tfmini.getDistance();
	return dist;
}

float SmoothLidarReading()
{
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
