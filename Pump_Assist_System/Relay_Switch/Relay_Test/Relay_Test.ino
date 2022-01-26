int Relaypin= 3; // Define input pin for relay signal

void setup() {
  // put your setup code here, to run once:
pinMode(Relaypin, OUTPUT); // Define the Relaypin as output pin
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(Relaypin, HIGH); // Sends high signal 
delay(1000); // Waits for 1 second
digitalWrite(Relaypin, LOW); // Makes the signal low
delay(1000); // Waits for 1 second
}
