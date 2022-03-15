#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <SPI.h>

#include <HPP_Controller.h>
#include <pump_config.h>

RH_RF95 driver(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(driver, SERVER_ADDRESS);

HPP_Controller pump_controller(1,2,3);

// control variables
int realThrottleState = 100;              // acceptable range: 0-100 (%)
int targetThrottleState = 100;  
int realRPMState = 3500;               // acceptable range: 0-4000 (RPM)
int realFuelState = 150;              // acceptable range: 0-100 (%)
int realKillStopState = 1;                        // 1 (true) = system ON, 0 (false) = system OFF
int targetKillStopState = 1;

// radio stuff
char statusMessage [63];    // initial status message

// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t receive_message[] = "Pump Controller: Received Status Message.";

void setup() {
  // Serial Setup
  while (!Serial) ; // Wait for serial port to be available
  Serial.begin(BAUD_RATE);
  delay(100);

  Serial.println("--- HPPA System: Pump Controller ---\n");
  Serial.println("Starting Pump Controller Setup");

  // other setup - TODO
  
  
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
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf); Serial.println("");

      // Send a reply back to the originator client
      if (!manager.sendtoWait(receive_message, sizeof(receive_message), from))
        Serial.println("Send response failed.");
    }
  }



  // do stuff here
  


  // set control values to random values within their respective range
  realThrottleState = rand() % (100 + 1 - 0);
  realRPMState = rand() % (4000 + 1 - 0);
  realFuelState = rand() % (100 + 1 - 0);
  realKillStopState = rand() % (1 + 1 - 0);

  // generate status message
  sprintf (statusMessage, "Throttle: %d%%. RPM: %drpm. Fuel Level: %d%%. KillSwitch: %d.",realThrottleState,\
                                                                                          realRPMState,\
                                                                                          realFuelState,\
                                                                                          realKillStopState);



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