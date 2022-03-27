
#define pulsePin 7                         //Pin used for Square Wave Input
int onTime, offTime;
float frequency, RPM, period; 

void setup() 
{
  Serial.begin(115200);                      //Establishes Serial connection w baudrate of 115200
  pinMode (pulsePin, INPUT);                 // Set Pin 7 as an input
}

void loop() 
{
  onTime = pulseIn (pulsePin, HIGH);         //Reads amount of time in microseconds that Pin is HIGH
                                             // HIGH > 3V (on 5V boards), HIGH > 2V (on 3.3V boards) 
  offTime = pulseIn (pulsePin, LOW);         //Reads amount of time in microseconds that Pin is LOW
                                             // LOW < 1.5V (on 5V boards), LOW < 1.0V (on 3.3V boards) 
  period = onTime + offTime; 
  frequency = 1000000/period;                //Converts square wave period to frequency
  RPM = frequency*60;                        //Revolutions/Minute

  Serial.print (int(RPM));
  Serial.println ("RPM");

   Serial.print (int(frequency));
  Serial.println ("Hz");

  delay (2000);
}
