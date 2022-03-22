#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <SPI.h>

#include <HPP_Controller.h>
#include <handheld_config.h>

RH_RF95 driver(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

HPP_Controller handheld_controller(1,2,3);

// control variables
int realThrottleState;              // acceptable range: 0-16
int targetThrottleState;  
int realRPMState;               // acceptable range: 0-4000 (RPM)
int realFuelState;              // acceptable range: 0-100 (%)
int realKillStopState;                        // 1 (true) = system ON, 0 (false) = system OFF
int targetKillStopState;

// radio stuff
char statusMessage [2];     // initial status message

// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t receive_message[] = "Handheld Controller: Received Status Message.";

// LCD Menu
// MENU ARRAYS
String menuOption[] = {{"Signal Freq."},{"Signal Power"}, {"Pump RPM:"}, {"Fuel Level"}, {"Cont.Batt.Level"}, {"Throttle Control"},{"Pump Kill Switch"}};   // Text on the top line 
String units[] = {{"MHz"},{"dBm"}, {"RPM"}, {"%"}, {"%"}, {"%"},{"ON"}};  // Text on the bottom line
String setOrNotSet[] = {{""},{""},{""},{""},{""},{""},{""}};

// LCD & I2C Declaration 
LiquidCrystal_I2C lcd(0x27, 16, 2); //Activates LCD "object" with I2C address and LCD dimensions

// Push Buttons
//Number Count of Button Presses and Counter Units
int setting0Counter = 0; //Counters for settings 0 - 6 
int setting1Counter = 0; 
int setting2Counter = 0;
int setting3Counter = 0;
int setting4Counter = 0;
int setting5Counter = 0;
int setting6Counter = 1; 
int directionPush = 0; //Counter to change menu option with each direction press
//Button Press States for each Function, All set to "HIGH" 
//When button not pressed (open), internal pullup resistor connects to 5V reporting "HIGH"
//When button pressed (closd), connection to ground is complete reporting "LOW"
boolean buttonStateLeft = HIGH;            // Button states for the "Left" command
boolean lastButtonStateLeft = HIGH;                
boolean currentButtonStateLeft = HIGH;
boolean buttonStateRight = HIGH;           // Button states for the "Right" command
boolean lastButtonStateRight = HIGH;                
boolean currentButtonStateRight = HIGH;
boolean buttonStateUp = HIGH;              // Button states for the "Up" command
boolean lastButtonStateUp = HIGH;                
boolean currentButtonStateUp = HIGH;
boolean buttonStateDown = HIGH;            // Button states for the "Down" command
boolean lastButtonStateDown = HIGH;                
boolean currentButtonStateDown = HIGH; 
// Debounce Variables to measure debounce time of a button push
//"Unsigned long" variable since the numerical value will have an extended size 
unsigned long lastDebounceTime = 0; //Takes a "snapshot" of time when any button is pushed
unsigned long debounceDelay = 50; //Delay time in milliseconds, amount of time the button is pressed must be higher than the delay to register as a push

void setup() {
  // SERIAL SETUP
  while (!Serial) ; // Wait for serial port to be available
  Serial.begin(BAUD_RATE);
  delay(100);

  Serial.println("--- HPPA System: Handheld Controller ---\n");
  Serial.println("Starting Handheld Controller Setup");

  // BUTTON SETUP
  //Initialize four buttons with pins and enable the internal pull-up resistor so that additional resistors are not required
  pinMode (leftButtonPin, INPUT_PULLUP);
  pinMode (rightButtonPin, INPUT_PULLUP);
  pinMode (upButtonPin, INPUT_PULLUP);
  pinMode (downButtonPin, INPUT_PULLUP);

  // DISPLAY SETUP
  lcd.backlight(); //Commands to turn on and clear LCD 
  lcd.init();
  lcd.setCursor(0,0);    //Menu displayed on the LCD 
  lcd.print("PumpAssistSystem");  //Menu option displayed is determined by left or right push
  lcd.setCursor(0,1);  //
  lcd.print ("..Initializing..");  // The setting counter that is displayed is determined by the left or right push.
  delay(2000);    // TO-DO - do we need this ...
  lcd.clear();

  // RADIO SETUP
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

  Serial.println("Completed Handheld Controller Setup\n\n");
}

void loop() {
  // receive pump status message
  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      // parse incoming status message
      realThrottleState = (int) buf[0];
      realFuelState = (int) buf[1];
      realKillStopState = (int) buf[2];
      realRPMState = (int) ( (buf[3] << 8) + (buf[4]));         // buf[3] = MSB, buf[4] = LSB, save rpmstate as 16byte value
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.print("Throttle: "); Serial.print(realThrottleState); Serial.print(". ");
      Serial.print("Fuel Level: "); Serial.print(realFuelState); Serial.print("%. ");
      Serial.print("Killswitch: "); Serial.print(realKillStopState); Serial.print(". ");
      Serial.print("RPM: "); Serial.print(realRPMState); Serial.println("rpm. ");

      // Send a reply back to the originator client
      if (!manager.sendtoWait(receive_message, sizeof(receive_message), from))
        Serial.println("Send response failed.");
    }
  }
  // do stuff here
  // update LCD display values
  setting0Counter = RF95_FREQ;
  setting1Counter = driver.lastRssi();
  setting2Counter = realRPMState;
  setting3Counter = realFuelState;


  // UPDATE BUTTON / LCD LOGIC
  int parameterValue[] = {setting0Counter, setting1Counter, setting2Counter, setting3Counter, setting4Counter, setting5Counter, setting6Counter};    // This variable holds the individual counters in one array

  lcd.setCursor(0,0);    //Menu displayed on the LCD 
  lcd.print(menuOption[directionPush]);  //Menu option displayed is determined by left or right push
  lcd.setCursor(0,1);
  lcd.print (parameterValue[directionPush]);  // The setting counter that is displayed is determined by the left or right push.
  lcd.print ("   ");
  lcd.setCursor(8,1);  //
  lcd.print (units[directionPush]);  // The setting counter that is displayed is determined by the left or right push.
  lcd.setCursor(12,1);
  lcd.print (setOrNotSet[directionPush]);   // print "SET!" message if target control value = real control value

  //Program now waits for button press.
  currentButtonStateLeft = digitalRead(leftButtonPin);          
  currentButtonStateRight = digitalRead(rightButtonPin); 
  currentButtonStateUp = digitalRead(upButtonPin);          
  currentButtonStateDown = digitalRead(downButtonPin);  

  if (currentButtonStateLeft != lastButtonStateLeft || currentButtonStateRight != lastButtonStateRight || 
  currentButtonStateUp != lastButtonStateUp || currentButtonStateDown != lastButtonStateDown)
  //If the lastDebounceTime (aka. the "snapshot" time) minus the running millis() function is higher than the set debounce delay, the following routine
  // below runs and checks which button was pushed: 
  {

  // The current state for each button is set not equal to the pressed state and when it changes, the pressed state becomes equal to the current state.
  // LEFT BUTTON PRESS
  if (currentButtonStateLeft != buttonStateLeft) // Left button scrolls the menu options to the left. 
  {
    buttonStateLeft = currentButtonStateLeft;  

    if(buttonStateLeft== HIGH) // Once the button is released, the push is registered and the code below runs.
    {    
      Serial.println("left button push");  //REMOVE                                            
      directionPush--;                                  
    }

    if (directionPush < 0)                           // If the user tries to scroll left of the first menu option,
        {                                                // the program will loop back to the last menu option.
        directionPush = 6;
        }
        lcd.clear();
  }

  // RIGHT BUTTON PRESS
    if (currentButtonStateRight != buttonStateRight)     // Right button scrolls the menu options to the right.    
    { 
    buttonStateRight = currentButtonStateRight;  
                  
      if (buttonStateRight == HIGH)                         
      {                                                 
      Serial.println("right button push");  //REMOVE
      directionPush++;                                   
      }
      if (directionPush > 6)                           // If the user tries to scroll above the last menu option,
        {                                                // the program will loop back to the first menu option.
        directionPush = 0;
        }
    lcd.clear();
    }

      // UP BUTTON PRESS
    if (currentButtonStateUp != buttonStateUp)           // Up button scrolls the counter setting upward. 
      { 
    buttonStateUp = currentButtonStateUp; 
      if (buttonStateUp == HIGH && directionPush == 5)    // each push will add AN increments to the throttle counter.  
      {       
      Serial.println("up button push");  //REMOVE             
      setting5Counter = setting5Counter + 1; 
      
      if (setting5Counter > 16)                       // Sets the setting5 counter limit to 100. The user cannot increase the counter beyond 100.
        {
        setting5Counter = 16;
        }
      }

      if (buttonStateUp == HIGH && directionPush == 6)                         
        {
          Serial.println("up button push");  //REMOVE
          setting6Counter = setting5Counter + 1;
          units[6] = {"ON"};
          if (setting6Counter > 1)                       // Sets the setting5 counter limit to 100. The user cannot increase the counter beyond 100.
          {
          setting6Counter = 1;
          }                                      
        } 

        lcd.clear();
      }

  // DOWN BUTTON PRESS
    if (currentButtonStateDown != buttonStateDown)        // Down button scrolls the setting downward.
    {
      buttonStateDown = currentButtonStateDown;  

      if (buttonStateDown == HIGH && directionPush == 5)   //  each push will subtract 10 increment from the throttle counter.                      
      {                                                   
        Serial.println("down button push");  //REMOVE 
        setting5Counter = setting5Counter - 1;

        if (setting5Counter < 0)                       // Sets the setting counter limit to 0 for now. The user cannot decrease the counter beyond 0.
          {
          setting5Counter = 0;
          }
      }
      if (buttonStateDown == HIGH && directionPush == 6)
      {
        Serial.println("down button push");  //REMOVE
        setting6Counter = setting6Counter - 1;
        units[6] = {"OFF"};
        if (setting6Counter < 0)
        {
          setting6Counter = 0;
        }
      }

    lcd.clear();
    }

    // After a button is pushed and the count recorded, all the states reset back to LOW for the data to be processed correctly.
  lastButtonStateLeft = currentButtonStateLeft;       // resets the left button state to LOW
  lastButtonStateRight = currentButtonStateRight;     // resets the right button state to LOW
  lastButtonStateUp = currentButtonStateUp;           // resets the up button state to LOW
  lastButtonStateDown = currentButtonStateDown;       // resets the down button state to LOW    
  }

  // update control variables
  targetThrottleState = setting5Counter;
  targetKillStopState = setting6Counter;            // 1 (true) = system ON, 0 (false) = system OFF
  
  // update "SET!" messages on LCD
  if (realThrottleState == targetThrottleState)
  {
    // display "SET!" message on LCD
    setOrNotSet[5] = {"SET!"};
  }
  else if (realThrottleState != targetThrottleState)
  {
    // clear "SET!" message on LCD
    setOrNotSet[5] = {"    "};
  }

  if (realKillStopState == targetKillStopState)
  {
    // display "SET!" message on LCD
    setOrNotSet[6] = {"SET!"};
  }
  else if (realKillStopState != targetKillStopState)
  {
    // clear "SET!" message on LCD
    setOrNotSet[6] = {"    "};
  }  

  // generate status message
  uint8_t statusMessage [] = { targetThrottleState,targetKillStopState };

  // send radio status message to pump controller 
  unsigned long radioCurrentTime = millis();
  if (radioCurrentTime - radioPreviousTime > radioStatusTransmitInterval)
  {
    radioPreviousTime = radioCurrentTime;

    // send radio status message to pump controller
    Serial.println("Sending status message to pump controller.");
      
    // Send a message to manager_server
    if (manager.sendtoWait( (uint8_t *)statusMessage, sizeof(statusMessage), SERVER_ADDRESS))
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
