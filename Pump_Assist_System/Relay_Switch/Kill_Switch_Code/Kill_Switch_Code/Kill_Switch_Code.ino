
int Relaypin= 7; // Define input pin for relay signal
int KillSwitch = 0;

void setup() 
{
  // put your setup code here, to run once:
pinMode(Relaypin, OUTPUT); // Define the Relaypin as output pin
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(Relaypin, LOW); // Sends Low signal (NO Switch = Open)

if (KillSwitch)
{
  digitalWrite(Relaypin, HIGH); // Sends High signal (NO Switch = Closed) 
  delay (10000);
}

}
