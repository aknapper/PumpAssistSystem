#include <DynamixelSerial.h>
#include <Arduino.h>

int ServoPos = 610;   //approx POS at start
int MaxPos = 661;   //Position at idle 
int MinPos = 389;   //Positiob at max power
int inc_dec = -1;
int deltaPOS = 17;  //Resolution per tick 

// control variables
long realThrottleState;              // acceptable range: 0-16
long targetThrottleState;  

void setThrottlePosition(long* targetThrottleState){
  // set servo resolution 661-389 given an integer 0-16 respectively
    Dynamixel.move(1,MaxPos - (deltaPOS * *targetThrottleState));
}

void getThrottlePosition(long* realThrottleState){
  /* Set realThrottle as an integer 0-16 based on the position of the servo.
     Result is rounded to avoid issues with the actual measured resolution sometimes being ~+/- 3 degrees the set value
  */
  *realThrottleState = round((MaxPos-Dynamixel.readPosition(1))/deltaPOS);
}

void setup(){
    // Serial Setup
    while (!Serial) ; // Wait for serial port to be available
    Serial.begin(9600);
    delay(100);

    Dynamixel.setSerial(&Serial1); // &Serial - Arduino UNO/NANO/MICRO, &Serial1, &Serial2, &Serial3 - Arduino Mega
    Dynamixel.begin(1000000,2);  // Initialize the servo at 1 Mbps and Pin Control 2
    delay(1000);
    Dynamixel.setEndless(1,OFF);
    Dynamixel.torqueStatus (1, OFF);
}

void loop(){

    targetThrottleState = rand() % (16 + 1 - 0);

    
    Serial.print("Target Throttle: "); Serial.println(targetThrottleState);
    Serial.print("Target Throttle (resolution): "); Serial.println(MaxPos - (deltaPOS * targetThrottleState));
    setThrottlePosition( &targetThrottleState);
    delay(3000);
    getThrottlePosition( &realThrottleState);
    Serial.print("Real Throttle Position: "); Serial.println(realThrottleState);
    Serial.print("Real Throttle Position (resolution): "); Serial.println(MaxPos-(deltaPOS*realThrottleState)); Serial.println("");

    delay(1000);
}
