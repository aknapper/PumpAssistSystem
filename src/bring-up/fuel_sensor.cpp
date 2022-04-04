#include <Arduino.h>

// Pin declaration
#define FUEL_SENSOR_PIN 0       // analog input

// Variables 
int AnalogVoltage = 0; 
int Vin = 5;
float DigitalVoltage = 0;
float R1 = 180.0;           //Value of Known Resistor is 180 Ohms
float R_Variable= 0;        // Variable Resistor Value ( 1 Ohm to 193 Ohms)
int fuelLevel = 0; 

void pollFuelSensor(int* fuelLevel) {
    AnalogVoltage= analogRead(FUEL_SENSOR_PIN);             // Reads voltage level ( 0 - 1023) across variable resistor
    Serial.print("Analog Voltage: "); Serial.println(AnalogVoltage);
        DigitalVoltage = (AnalogVoltage/1024.0)* Vin ;      // Analog voltage across variable resistor 
        R_Variable = R1 * ((Vin/DigitalVoltage) -1);        // Determines Resistance across variable resistor
        *fuelLevel = map((int) (0.520833 * (R_Variable - 1))*100,200,10350,0,100);    // Calculates Fuel Level in %, represented by linear equation: y=mx+b 
}

void setup() {
    // Serial Setup
    while (!Serial) ; // Wait for serial port to be available
    Serial.begin(9600);
    delay(100);
    Serial.println("Fuel Sensor Test..."); Serial.println();
}

void loop() {
    pollFuelSensor( &fuelLevel);
    Serial.print("Fuel Level: "); Serial.println(fuelLevel);
    delay(1000);
}