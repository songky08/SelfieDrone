#include <SoftwareSerial.h>
#include <math.h>
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

SoftwareSerial bluetooth(0,1);    // RX : Digital Pin 0, TX : Digital Pin 1

MPU6050 accelgyro;
double dt = 0.043, dt2;
int g_duty;
char rx;
int16_t ax, ay, az;
int16_t gx_r, gy_r, gz_r;
double gx, gy, gz;
double gx_s, gy_s;
int32_t mx, my, mz;
int32_t gx_c, gy_c, gz_c;
double a_roll, a_pitch;
double roll, pitch, yaw;
double roll_std, pitch_std;
double roll_err, pitch_err;

void trimAngle()
{
  double roll_sum = 0, pitch_sum = 0;
  double roll_ave, pitch_ave;
  for (int i = 0; i < 10; i++)
  {
    accelgyro.getMotion6(&ax, &ay, &az, &gx_r, &gy_r, &gz_r);
    a_roll = atan2(ay, sqrt(pow(ax, 2) + pow(az, 2))) * 180 / 3.141592;
    a_pitch = -atan2(ax, sqrt(pow(ay, 2) + pow(az, 2))) * 180 / 3.141592;
    
    roll_sum += a_roll;
    pitch_sum += a_pitch;
  }
  roll_ave = roll_sum / 10;
  pitch_ave = pitch_sum / 10;
  roll_std = roll_ave;
  pitch_std = pitch_ave;
}
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
  InitMotor();
  InitI2C();
  SetUpMotor();

  initBluetooth();

  SetPin3_Duty(0);//min = 2, max = 135
  SetPin5_Duty(0);//min = 2, max = 135
  SetPin9_Duty(0);//min = 2, max = 135
  SetPin10_Duty(0);//min = 2, max = 135
//  Serial.begin(9600);
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
  trimAngle();
}

void loop() {
  static double r_p, r_i, r_d, r_p_g, r_i_g, r_d_g, p_p, p_i, p_d, p_p_g, p_i_g, p_d_g;
  static unsigned int throttle = 10;
  rx = getCommand();



  // these methods (and a few others) are also available
  accelgyro.getMotion6(&ax, &ay, &az, &gx_r, &gy_r, &gz_r);
  a_roll = atan2(ay, sqrt(pow(ax, 2) + pow(az, 2))) * 180 / 3.141592;
  a_pitch = -atan2(ax, sqrt(pow(ay, 2) + pow(az, 2))) * 180 / 3.141592;
  gx = (double)(gx_r - gx_c) / (double)131;
  gy = (double)(gy_r - gy_c) / (double)131;
  gz = (double)(gz_r - gz_c) / (double)131;
  
  gx_s += gx * dt;
  gy_s += gy * dt;
  roll = 0.95 * (roll + gx * dt) + 0.05 * a_roll;
  pitch = 0.95 * (pitch + gy * dt) + 0.05 * a_pitch;
  roll_err = roll - roll_std;
  pitch_err = pitch - pitch_std;
  r_p = roll_err * r_p_g;
  r_i += roll_err * dt * r_i_g;
  r_d = gx * r_d_g;
  p_p = pitch_err * p_p_g;
  p_i += pitch_err * dt * p_i_g;
  p_d = gy * p_d_g;
  
/*  07/14 roll, Kp = 0.40, Ki = 0.06, Kd = 1.0  */
/*  07/15 roll, Kp = 0.35, Ki = 0.08, Kd = 1.3  */
  SetPin3_Duty(throttle + r_p + r_i + r_d);
  SetPin5_Duty(throttle - r_p - r_i - r_d);
/*  07/15 pitch Kp = 0.35, Ki = 0.08, Kd = 1.3  */
  SetPin9_Duty(throttle + p_p + p_i + p_d);
  SetPin10_Duty(throttle - p_p - p_i - p_d);
  
    
  switch(rx)
  {
    case 'q':
    r_p_g = 0.35;
    p_p_g = 0.35;
    r_i_g = 0.08;
    p_i_g = 0.08;
    r_d_g = 1.3;
    p_d_g = 1.3;
    bluetooth.println("PID on");
    break;
    
    case 'u':
    throttle++;
    bluetooth.print("throttle : ");
    bluetooth.println(throttle);
    break;
    
    case 'y':
    throttle--;
    bluetooth.print("throttle : ");
    bluetooth.println(throttle);
    break;
    
    case 't':
    throttle = 0;
    r_p_g = 0;
    r_i_g = 0;
    r_d_g = 0;
    p_p_g = 0;
    p_i_g = 0;
    p_d_g = 0;
    bluetooth.println("turn off");
    break;
    
    case 'p':
    p_p_g += 0.1;
    r_p_g += 0.1;
    bluetooth.print("p_p_g : ");
    bluetooth.println(p_p_g);
    break;
    
    case '0':
    p_p_g += 0.01;
    r_p_g += 0.01;
    bluetooth.print("p_p_g : ");
    bluetooth.println(p_p_g);
    break;
    
    case 'l':
    p_p_g -= 0.1;
    r_p_g -= 0.1;
    bluetooth.print("p_p_g : ");
    bluetooth.println(p_p_g);
    break;
    
    case '9':
    p_p_g -= 0.01;
    r_p_g -= 0.01;
    bluetooth.print("p_p_g : ");
    bluetooth.println(p_p_g);
    break;
    
    case 'i':
    p_i_g += 0.1;
    r_i_g += 0.1;
    bluetooth.print("p_i_g : ");
    bluetooth.println(p_i_g);
    break;
    
    case '8':
    p_i_g += 0.01;
    r_i_g += 0.01;
    bluetooth.print("p_i_g : ");
    bluetooth.println(p_i_g);
    break;
    
    case 'k':
    p_i_g -= 0.1;
    r_i_g -= 0.1;
    bluetooth.print("p_i_g : ");
    bluetooth.println(p_i_g);
    break;
    
    case '7':
    p_i_g -= 0.01;
    r_i_g -= 0.01;
    bluetooth.print("p_i_g : ");
    bluetooth.println(p_i_g);
    break;
    
    case 'd':
    p_d_g += 0.1;
    r_d_g += 0.1;
    bluetooth.print("p_d_g : ");
    bluetooth.println(p_d_g);
    break;
    
    case 'e':
    p_d_g += 0.01;
    r_d_g += 0.01;
    bluetooth.print("p_d_g : ");
    bluetooth.println(p_d_g);
    break;
    
    case 'c':
    p_d_g -= 0.1;
    r_d_g -= 0.1;
    bluetooth.print("p_d_g : ");
    bluetooth.println(p_d_g);
    break;
    
    case 'w':
    p_d_g -= 0.01;
    r_d_g -= 0.01;
    bluetooth.print("p_d_g : ");
    bluetooth.println(p_d_g);
    break;
    
    case 'a':
    bluetooth.print("roll_err : ");
    bluetooth.print(roll_err);
    bluetooth.print("\tpitch_err : ");
    bluetooth.print(pitch_err);
  }
}
