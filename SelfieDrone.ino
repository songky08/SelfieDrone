#include <SoftwareSerial.h>
#include <math.h>
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

SoftwareSerial bluetooth(0,1);    // RX : Digital Pin 0, TX : Digital Pin 1

MPU6050 accelgyro;
double dt;
int g_duty;
char rx;
int16_t ax, ay, az;
int16_t gx_r, gy_r, gz_r;
double gx, gy, gz;
int32_t mx, my, mz;
int32_t gx_c, gy_c, gz_c;
double a_roll, a_pitch;
double roll, pitch, yaw;
void initBluetooth() 
{
  bluetooth.begin(115200);
  bluetooth.write("handshake");
}

char getCommand()
{
  
  if(bluetooth.available()>0)
  {
    return bluetooth.read();  
  }
  else 
    return -1;
}

void setup() {
  // put your setup code here, to run once:
  
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
//  InitMotor();
  InitI2C();
//  SetUpMotor();
//  initBluetooth();
  SetPin3_Duty(0);//min = 4, max = 248
  SetPin5_Duty(0);
  SetPin9_Duty(0);
  SetPin10_Duty(0);
  Serial.begin(9600);
}

void loop() {
  
//  rx = getCommand();

  accelgyro.getMotion6(&ax, &ay, &az, &gx_r, &gy_r, &gz_r);
  accelgyro.getMotion6(&ax, &ay, &az, &gx_r, &gy_r, &gz_r);
  accelgyro.getMotion6(&ax, &ay, &az, &gx_r, &gy_r, &gz_r);
  for(int i = 0; i < 10; i++)
  {
    accelgyro.getMotion6(&ax, &ay, &az, &gx_r, &gy_r, &gz_r);
    gx_c += gx_r;
    gy_c += gy_r;
    gz_c += gz_r;
  }
  gx_c /= 10;
  gy_c /= 10;
  gz_c /= 10;

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);
  a_roll = atan2(ay, sqrt(pow(ax, 2) + pow(az, 2))) * 180 / 3.141592;
  a_pitch = -atan2(ax, sqrt(pow(ay, 2) + pow(az, 2))) * 180 / 3.141592;
  gx = (double)(gx_r - gx_c) / (double)131;
  gy = (double)(gy_r - gy_c) / (double)131;
  gz = (double)(gz_r - gz_c) / (double)131;

  dt = millis() - dt;
  dt /= 100;
  roll = 0.95 * (roll - gx * dt) + 0.05 * a_roll;
  pitch = 0.95 * (pitch - gy * dt) + 0.05 * a_pitch;
  dt = millis();
  
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a_roll: ");
  Serial.print(a_roll); Serial.print("\t");
  Serial.print("roll: ");
  Serial.print(roll); Serial.print("\t");
  Serial.print("a_pitch: ");
  Serial.print(a_pitch); Serial.print("\t");
  Serial.print("pitch: ");
  Serial.println(pitch);
//  switch(rx)
//  {
//    case 'u':
//      SetPin3_Duty(++g_duty);
//      SetPin5_Duty(++g_duty);
//      SetPin9_Duty(++g_duty);
//      SetPin10_Duty(++g_duty);
//    break;
//    case 'd':
//      SetPin3_Duty(--g_duty);
//      SetPin5_Duty(--g_duty);
//      SetPin9_Duty(--g_duty);
//      SetPin10_Duty(--g_duty);
//    break;
//  }

  // put your main code here, to run repeatedly:
}
