unsigned long start;
unsigned long elapsed;
unsigned long RPM;
int numOfCyl = 2 ;                                                    // 2 - Stroke Motor
int pin = 3;                                                          // digital pins 2,3,18,19,20 or 21 can be used on mega for ISR

void setup()
{
  Serial.begin(115200);                                               //Establishes Serial connection w baudrate of 115200
  pinMode(pin, INPUT);                                                // Configures PIN to act as INPUT
  digitalWrite(pin, HIGH);                                            // HIGH = enable internal pullup resistor
  attachInterrupt(digitalPinToInterrupt(pin), speedCalc, LOW);    // 1. Pin used for interrupt, 2. SpeedCalc = ISR to call when interrupt occurs, 3. FALLING = mode when interrupt should be triggered
  start = micros();                                                   //Micros () will return the number of microseconds since the arduino began running the current program
}

void speedCalc()  
{
  elapsed = micros() - start;                                        // Calculates time elapsed since last ISR Call 
  start = micros();                                                  // Reset start time 

  
  RPM = 60*(1/(elapsed*1000000));                                   //uS  +- 3RPM resolution, calculate RPM from elapsed time 
}

void loop()
{
  Serial.print(int(RPM));                                          // Prints RPM value to serial monitor
  Serial.print(" rpm ");

  Serial.println(elapsed);                                         // Prints elapsed time **Not needed 
  Serial.println(" ms/rev");
  delay(1000);

 if ((micros()-start) >= 500000)                                 //If elapsed time >= 50ms, RPM = 0 
  {
     RPM=0;                                                      //if RPM times out then RPM = 0
  }

  
}
