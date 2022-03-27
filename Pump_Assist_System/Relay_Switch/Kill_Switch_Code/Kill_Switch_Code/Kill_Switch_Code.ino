
int RelayPin= 7; // Define input pin for relay signal
int KillSwitch = 0;

void setup() 
{
  // put your setup code here, to run once:
pinMode(RelayPin, OUTPUT); // Define the Relaypin as output pin
}

void loop() {
  
digitalWrite(RelayPin, LOW); // Sends Low signal (NO Switch = Open)

if (KillSwitch)               //if statement will run when KillSwitch not equal to one
{
  digitalWrite(RelayPin, HIGH); // Sends High signal (NO Switch = Closed) 
  delay (10000);
}

}
