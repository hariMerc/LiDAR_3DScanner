#include <PWMServo.h>
#include <SoftwareSerial.h>
#define LIDAR_MIN_DIST 5
#define LIDAR_MAX_DIST 900 // in cm
#define SAMPLES 5

SoftwareSerial Serial1(2, 3);
PWMServo hServo;  // Create servo object for horizontal servo
PWMServo vServo;    // Create servo object for vertical servo

const int HEADER = 0x59;
int dist;
int i;
int check;
int pointArray[2];//r, theta, phi
int uart[9];
int mapAngle;
int mapVerAngle;
int hAngle;   // Variable to store horizontal servo position
int vAngle = 180;     // Variable to store vertical servo position
//int horizonDelay = 50;

void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);
  hServo.attach(10);  // Attach horizontal servo to pin 9
  vServo.attach(9);    // Attach vertical servo to pin 10
  vServo.write(180);
  hServo.write(vAngle);
}

void loop() {
  
  if (vAngle <= 180){
    for(hAngle = 0; hAngle <= 180; hAngle++)
    {
      pointArray[2] = SmoothLidarReading();
      pointArray[0] = hAngle;
      printArray();
      hServo.write(hAngle);
      delay(200);
      
    }
    moveUp();
    for(hAngle = 180; hAngle >= 0; hAngle-=1)
    {

      pointArray[2] = SmoothLidarReading();
      pointArray[0] = hAngle;
      printArray();
      hServo.write(hAngle);
      delay(200);
      
    }
    moveUp();
  }

}

void moveUp() {
    if(vAngle <= 180 ){
      vAngle -= 5;
      mapVerAngle = map(vAngle, 180, 90, 0, 90);
      vServo.write(vAngle);
      pointArray[1] = mapVerAngle;
      delay(200);
    }
}

int LidarReading()
{
  if (Serial1.available()) {  //check if serial port has data input
    if(Serial1.read() == HEADER) {  //assess data package frame header 0x59
      uart[0]=HEADER;
      if (Serial1.read() == HEADER) { //assess data package frame header 0x59
        uart[1] = HEADER;
        for (i = 2; i < 9; i++) { //save data in array
          uart[i] = Serial1.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff)){ //verify the received data as per protocol
          dist = uart[2] + uart[3] * 256;     //calculate distance value
          return dist;
        }
      }
    }
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

void printArray() {
  for (int i = 0; i < 3; i++) {
    Serial.print(pointArray[i]);
    if (i != 3) {
      Serial.print(" ");
    }
  }
  Serial.println();
}
