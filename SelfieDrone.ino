#include <SoftwareSerial.h>

SoftwareSerial bluetooth(0,1);    // RX : Digital Pin 0, TX : Digital Pin 1

int g_duty;
char g_rx;

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
//  SetUpMotor();
  initBluetooth();
  SetPin3_Duty(0);//min = 4, max = 248
  SetPin5_Duty(0);
  SetPin9_Duty(0);
  SetPin10_Duty(0);
}

void loop() {
  g_rx = getCommand();
  switch(g_rx)
  {
    case 'u':
      SetPin3_Duty(++g_duty);
      SetPin5_Duty(++g_duty);
      SetPin9_Duty(++g_duty);
      SetPin10_Duty(++g_duty);
    break;
    case 'd':
      SetPin3_Duty(--g_duty);
      SetPin5_Duty(--g_duty);
      SetPin9_Duty(--g_duty);
      SetPin10_Duty(--g_duty);
    break;
  }

  // put your main code here, to run repeatedly:
}
