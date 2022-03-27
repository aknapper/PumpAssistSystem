#include <DynamixelSerial.h>

void setup(){
Dynamixel.setSerial(&Serial); // &Serial - Arduino UNO/NANO/MICRO, &Serial1, &Serial2, &Serial3 - Arduino Mega
Dynamixel.begin(1000000,2);  // Initialize the servo at 1 Mbps and Pin Control 2
delay(1000);
Dynamixel.setEndless(1,OFF);
 Dynamixel.torqueStatus (1, OFF);
}

int ServoPos = 610;   //approx POS at start
int MaxPos = 661;   //Position at idle 
int MinPos = 389;   //Positiob at max power
int inc_dec = -1;
int deltaPOS = 17;  //Resolution per tick 

void loop()
{
  
  Dynamixel.move(1,ServoPos);  // Move the Servo to starting position
  Dynamixel.ledStatus(1,ON);
  delay (1000);
  ServoPos = ServoPos + (inc_dec*deltaPOS);
  
  if (ServoPos == MaxPos || ServoPos ==MinPos)
  {
  inc_dec = inc_dec*(-1); 
  }

 
  
  Dynamixel.ledStatus(1,OFF);
  delay (1000);
  //Dynamixel.moveRW(1,512);
  //delay(1000);
  //Dynamixel.action();
  //Dynamixel.ledStatus(1,OFF);
 
//delay(1000);

}
