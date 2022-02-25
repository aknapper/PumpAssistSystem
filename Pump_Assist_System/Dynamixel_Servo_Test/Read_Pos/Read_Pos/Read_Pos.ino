#include <DynamixelSerial.h>

int Position;

void setup(){
Dynamixel.setSerial(&Serial); // &Serial - Arduino UNO/NANO/MICRO, &Serial1, &Serial2, &Serial3 - Arduino Mega
Dynamixel.begin(1000000,2);  // Inicialize the servo at 1 Mbps and Pin Control 2
delay(1000);
}

void loop(){
  
  Position = Dynamixel.readPosition(1);       // Request and Print the Position
 
 Dynamixel.end();                 // End Servo Comunication
 Serial.begin(9600);              // Begin Serial Comunication
 
  Serial.print(Position);
  Serial.println(" of 1023 resolution");
  
 Serial.end();                     // End the Serial Comunication
 Dynamixel.begin(1000000,2);         // Begin Servo Comunication
 
delay(5000);

}
