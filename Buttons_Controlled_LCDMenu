// Libraries
#include <Wire.h>               // deals with I2C connections
#include <LiquidCrystal_I2C.h>  // activates the LCD I2C library

// MENU ARRAYS
String menuOption[] = {{"Signal Freq."},{"Signal Power"}, {"Pump RPM:"}, {"Fuel Level"}, {"Cont.Batt.Level"},{"Pump.Batt.Level"}, {"Throttle Control"},{"Pump Kill Switch"}};   // Text on the top line 
String units[] = {{"MHz"},{"dBm"}, {"RPM"}, {"%"}, {"%"},{"%"}, {"%"},{"OFF"}};  // Text on the bottom line

//Pins on Arduino
const int leftButtonPin = 2;
const int rightButtonPin = 3;
const int upButtonPin = 4;
const int downButtonPin = 5;

//Number Count of Button Presses and Counter Units
int setting0Counter = 0; //Counters for settings 1 - 6 
int setting1Counter = 0;
int setting2Counter = 0;
int setting3Counter = 0;
int setting4Counter = 0;
int setting5Counter = 0;
int setting6Counter = 0; 
int setting7Counter = 0; 
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

// LCD & I2C Declaration 
LiquidCrystal_I2C lcd(0x27, 16, 2); //Activates LCD "object" with I2C address and LCD dimensions

//SETUP CODE
void setup() {
  
 lcd.backlight(); //Commands to turn on and clear LCD 
 lcd.init();

//Initialize four buttons with pins and enable the internal pull-up resistor so that additional resistors are not required
 pinMode (leftButtonPin, INPUT_PULLUP);
 pinMode (rightButtonPin, INPUT_PULLUP);
 pinMode (upButtonPin, INPUT_PULLUP);
 pinMode (downButtonPin, INPUT_PULLUP);


 lcd.setCursor(0,0);    //Menu displayed on the LCD 
 lcd.print("PumpAssistSystem");  //Menu option displayed is determined by left or right push
 lcd.setCursor(0,1);  //
 lcd.print ("..Initialising..");  // The setting counter that is displayed is determined by the left or right push.

 delay(5000);
 lcd.clear();
}

//MAIN LOOP
void loop() {
  
 int parameterValue[] = {setting0Counter, setting1Counter, setting2Counter, setting3Counter, setting4Counter, setting5Counter, setting6Counter, setting7Counter};    // This variable holds the individual counters in one array

 lcd.setCursor(0,0);    //Menu displayed on the LCD 
 lcd.print(menuOption[directionPush]);  //Menu option displayed is determined by left or right push
 lcd.setCursor(0,1);  //
 lcd.print (parameterValue[directionPush]);  // The setting counter that is displayed is determined by the left or right push.
 lcd.setCursor(8,1);  //
 lcd.print (units[directionPush]);  // The setting counter that is displayed is determined by the left or right push.

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
      directionPush--;                                  
    }

   if (directionPush < 0)                           // If the user tries to scroll left of the first menu option,
        {                                                // the program will loop back to the last menu option.
        directionPush = 7;
        }
        lcd.clear();
 }

  // RIGHT BUTTON PRESS
    if (currentButtonStateRight != buttonStateRight)     // Right button scrolls the menu options to the right.    
    { 
    buttonStateRight = currentButtonStateRight;  
                 
      if (buttonStateRight == HIGH)                         
      {                                                 
      directionPush++;                                   
      }
      if (directionPush > 7)                           // If the user tries to scroll above the last menu option,
        {                                                // the program will loop back to the first menu option.
        directionPush = 0;
        }
    lcd.clear();
    }

     // UP BUTTON PRESS
    if (currentButtonStateUp != buttonStateUp)           // Up button scrolls the counter setting upward. 
     { 
    buttonStateUp = currentButtonStateUp; 
     if (buttonStateUp == HIGH && directionPush == 6)    // each push will add 10 increments to the throttle counter.  
      {                    
      setting6Counter = setting6Counter + 10; 
      
      if (setting6Counter > 100)                       // Sets the setting5 counter limit to 100. The user cannot increase the counter beyond 100.
        {
        setting6Counter = 100;
        }
    }

    if (buttonStateUp == HIGH && directionPush == 7)                         
      {                                                       
      lcd.clear();                      //Display to user that the Pump Kill switch has been engaged
      lcd.setCursor(0,0);    // 
      lcd.print("Pump Kill Switch");  //
      lcd.setCursor(8,1);  //
      lcd.print ("ON");  // 
      delay (10000);                   // Switch is engaged for 10 seconds before being automatically shut off
      } 

       lcd.clear();
     }

  // DOWN BUTTON PRESS
    if (currentButtonStateDown != buttonStateDown)        // Down button scrolls the setting downward.
    {
      buttonStateDown = currentButtonStateDown;  

      if (buttonStateDown == HIGH && directionPush == 6)   //  each push will subtract 10 increment from the throttle counter.                      
      {                                                   
      setting6Counter = setting6Counter - 10;

      if (setting6Counter < 0)                       // Sets the setting counter limit to 0 for now. The user cannot decrease the counter beyond 0.
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
}
