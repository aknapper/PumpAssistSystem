
// Pin declaration
 int Fuel_Sensor_Pin = 0;      // Fuel Sensor assigned to Analog Pin 0

// Variables 
 int AnalogVoltage = 0; 
 int Vin = 5;
 float DigitalVoltage = 0;
 float R1 = 180.0;           //Value of Known Resistor is 180 Ohms
 float R_Variable= 0;        // Variable Resistor Value ( 1 Ohm to 193 Ohms)
 float FuelLevel = 0;        
 

void FuelSensorMain()
  {
AnalogVoltage= analogRead(Fuel_Sensor_Pin);  //Reads voltage level ( 0 - 1023) across variable resistor
if(DigitalVoltage)                            //Only executes instructions DigitalVoltage not equal to 0 
  {
DigitalVoltage = (AnalogVoltage/1024.0)* Vin ;  // Analog voltage across variable resistor 
R_Variable = R1 * ((Vin/DigitalVoltage) -1);     // Determines Resistance across variable resistor
FuelLevel = (0.520833 * (R_Variable - 1)) * 100;        //Calculates Fuel Level in %, represented by linear equation: y=mx+b 

}
return (FuelLevel);
}
