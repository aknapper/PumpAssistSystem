#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <SPI.h>
#include <DynamixelSerial.h>

#include <HPP_Controller.h>
#include <pump_config.h>

RH_RF95 driver(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(driver, SERVER_ADDRESS);

HPP_Controller pump_controller(1,2,3);

// control variables
long realThrottleState;              // acceptable range: 0-16
long targetThrottleState;  
int realRPMState;               // acceptable range: 0-4000 (RPM)
int realFuelState;              // acceptable range: 0-100 (%)
int realKillStopState;                        // 1 (true) = system ON, 0 (false) = system OFF
int targetKillStopState;

// radio stuff
char statusMessage [5];    // initial status message

// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t receive_message[] = "Pump Controller: Received Status Message.";

// fuel sensor variables
int AnalogVoltage = 0; 
int Vin = 5;
float DigitalVoltage = 0;
float R1 = 180.0;           //Value of Known Resistor is 180 Ohms
float R_Variable= 0;        // Variable Resistor Value ( 1 Ohm to 193 Ohms)

void pollFuelSensor(int* fuelLevel) {
    AnalogVoltage= analogRead(FUEL_SENSOR_PIN);             // Reads voltage level ( 0 - 1023) across variable resistor
    // Serial.print("Analog Voltage: "); Serial.println(AnalogVoltage);
    DigitalVoltage = (AnalogVoltage/1024.0)* Vin ;      // Analog voltage across variable resistor 
    R_Variable = R1 * ((Vin/DigitalVoltage) -1);        // Determines Resistance across variable resistor
    *fuelLevel = map((int) (0.520833 * (R_Variable - 1))*100,200,10350,0,100);    // Calculates Fuel Level in %, represented by linear equation: y=mx+b 
}

// servo variables
int MaxPos = 661;   //Position at idle 
int deltaPOS = 17;  //Resolution per tick

void setThrottlePosition(long* targetThrottleState){
  // set servo resolution 661-389 given an integer 0-16 respectively
    Dynamixel.move(1,MaxPos - (deltaPOS * *targetThrottleState));
    Dynamixel.torqueStatus (1, OFF);
}

void getThrottlePosition(long* realThrottleState){
  /* Set realThrottle as an integer 0-16 based on the position of the servo.
     Result is rounded to avoid issues with the actual measured resolution sometimes being ~+/- 3 degrees the set value
  */
  *realThrottleState = round((MaxPos-Dynamixel.readPosition(1))/deltaPOS);
}

void setEmergencyStop(int* targetKillStopState){
  if (*targetKillStopState){                    // is 1 (True)
    digitalWrite(RELAY_PIN, HIGH);              // set relay pin high (pump on / e-stop not on)
  }
  else if (!*targetKillStopState){              // is 0 (False)
    digitalWrite(RELAY_PIN, LOW);               // set relay pin low (pump off / e-stop on)
  }
}

void getEmergencyStop(int* realKillStopState){
  *realKillStopState = digitalRead(RELAY_PIN);  // sets realKillStopState to 1 or 0 (True or False)
}

void setup() {
  // Serial Setup
  while (!Serial) ; // Wait for serial port to be available
  Serial.begin(BAUD_RATE);
  delay(100);

  Serial.println("--- HPPA System: Pump Controller ---\n");
  Serial.println("Starting Pump Controller Setup");

  // other setup - TODO
  // relay setup
  pinMode(RELAY_PIN, OUTPUT);         // set e-stop OFF, pump ON

  // servo setup
  Dynamixel.setSerial(&Serial1); // &Serial1 -> TX1/RX1 : D18/D19
  Dynamixel.begin(DYNAMIXEL_BAUDRATE,DYNAMIXEL_CONTROL_PIN);  // Initialize the servo at 1 Mbps and Pin Control 2
  delay(500);
  Dynamixel.setEndless(1,OFF);
  
  // radio setup
  Serial.println("Initializing RRM95 Radio");
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  if (!manager.init())
    Serial.println("init failed");
  Serial.println("LoRa radio initialization OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!driver.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Radio Frequency to: "); Serial.print(RF95_FREQ); Serial.println("Hz");
  driver.setTxPower(RF95_TX_POWER, false);
  Serial.print("Radio transmit power set to: "); Serial.print(RF95_TX_POWER); Serial.println("dBm");
  
  Serial.println("Completed radio setup.");


  Serial.println("Completed Pump Controller Setup\n\n");
}

void loop() {
  // receive handheld status message
  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      targetThrottleState = (int) buf[0];
      targetKillStopState = (int) buf[1];
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.print("Throttle: "); Serial.print((int) buf[0]); Serial.print(". ");
      Serial.print("Killswitch: "); Serial.print((int) buf[1]); Serial.println(". ");


      // Send a reply back to the originator client
      if (!manager.sendtoWait(receive_message, sizeof(receive_message), from))
        Serial.println("Send response failed.");
    }
  }

  // do stuff here
  // throttle (servo) stuff
  setThrottlePosition( &targetThrottleState);
  getThrottlePosition( &realThrottleState);

  // killswitch (relay) stuff
  setEmergencyStop( &targetKillStopState);
  getEmergencyStop( &realKillStopState);

  // fuel sensor stuff
  pollFuelSensor( &realFuelState);

  // RPM sensor stuff


  // set control values to random values within their respective range
  // realThrottleState = rand() % (16 + 1 - 0);
  realRPMState = rand() % (4000 + 1 - 0);
  // realFuelState = rand() % (100 + 1 - 0);
  // realKillStopState = rand() % (1 + 1 - 0);

  // generate status message
  uint8_t statusMessage [] = { realThrottleState, realFuelState, realKillStopState,\
                               (uint8_t) (realRPMState >> 8), (uint8_t) realRPMState};

  // send radio status message to handheld controller 
  unsigned long radioCurrentTime = millis();
  if (radioCurrentTime - radioPreviousTime > radioStatusTransmitInterval)
  {
    radioPreviousTime = radioCurrentTime;

    // send radio status message to pump controller
    Serial.println("Sending status message to pump controller.");
      
    // Send a message to manager_server
    if (manager.sendtoWait( (uint8_t *)statusMessage, sizeof(statusMessage), CLIENT_ADDRESS))
    {
      // Now wait for a reply from the server
      uint8_t len = sizeof(buf);
      uint8_t from;   
      if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
      {
        Serial.print("got reply from : 0x");
        Serial.print(from, HEX);
        Serial.print(": ");
        Serial.println((char*)buf);
        Serial.print("RSSI: ");
        Serial.println(driver.lastRssi(), DEC);
      }
      else
      {
        Serial.println("No reply, is pump controller running?");
      }
    }
    else
      Serial.println("Status message send failed"); Serial.println("");
  }



  // heartbeat
  unsigned long heartbeatCurrentTime = millis();
  if (heartbeatCurrentTime - heartbeatPreviousTime > heartbeatInterval)
  {
    heartbeatPreviousTime = heartbeatCurrentTime;
    Serial.println("Heartbeat"); Serial.println("");
  }
}