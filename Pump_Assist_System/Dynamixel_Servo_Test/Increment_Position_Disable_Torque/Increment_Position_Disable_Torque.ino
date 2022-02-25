#include <DynamixelSerial.h>

void setup(){
Dynamixel.setSerial(&Serial); // &Serial - Arduino UNO/NANO/MICRO, &Serial1, &Serial2, &Serial3 - Arduino Mega
Dynamixel.begin(1000000,2);  // Initialize the servo at 1 Mbps and Pin Control 2
delay(1000);
Dynamixel.setEndless(1,OFF);
}

int ServoPos = 50;
int MaxPos = 950;
int MinPos = 50;
int inc_dec = 1;
void loop(){
  
  Dynamixel.move(1,ServoPos);  // Move the Servo to starting position
  Dynamixel.ledStatus(1,ON);
  delay (2000);
  ServoPos = ServoPos + (inc_dec*50);
  
  if (ServoPos == MaxPos || ServoPos ==MinPos)
  {
  inc_dec = inc_dec*(-1); 
  }

 
  
  Dynamixel.ledStatus(1,OFF);
  delay (2000);
  //Dynamixel.moveRW(1,512);
  //delay(1000);
  //Dynamixel.action();
  //Dynamixel.ledStatus(1,OFF);
 
//delay(1000);

}
