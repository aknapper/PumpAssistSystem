// LIBRARIES
#include "LiquidCrystal_I2C.h"  //Activates the LCD I2C Library
#include "Wire.h" //deals with I2C Connections

// Object Declaration
LiquidCrystal_I2C lcd(0x27, 16, 2); // Activates I2C Device Address and LCD Display


bool menu1_Selected = false; //enter/exit menu1
bool menu2_Selected = false; //enter/exit menu2
bool menu3_Selected = false; //enter/exit menu3
bool menu4_Selected = false; //enter/exit menu4
bool menu5_Selected = false; //enter/exit menu5
bool menu6_Selected = false; //enter/exit menu6
//If menu is selected, value beomes true

//define four different variables with pin number to know what signal the different buttons send their state to the arduino
const int buttonPinLeft = 2;  // Left button to Pin 2
const int buttonPinDown = 3;  // Down button to Pin 3
const int buttonPinUp = 4;  //Up button to Pin 4
const int buttonPinRight = 5; //Right button to Pin 5



// LCD Menu Logic
const int numOfSubmenus = 6; //Number of rows in main menu
int menuCounter = 0;  //Row position in main menu 
String mainmenu[numOfSubmenus][1] = {{"Signal Parameters"},{"Pump RPM"},{"Fuel Level"},{"Battery Level"},{"Throttle Control"},{"Pump Kill Switch"}};  //Submenus

bool refreshLCD = true; //refreshes values 
bool refreshSelection = false; //refreshes selection

void setup() {
Serial.begin(9600);
lcd.init(); //Initialize the LCD 
lcd.backlight(); //Enable/Turn-on backlight

//Initialize four buttons with pins and enable the internal pull-up resistor so that additional resistors are not required
pinMode(buttonPinLeft, INPUT_PULLUP);
pinMode(buttonPinDown, INPUT_PULLUP);
pinMode(buttonPinUp, INPUT_PULLUP);
pinMode(buttonPinRight, INPUT_PULLUP);
//-------------------------------------------
lcd.clear();
lcd.setCursor(0,0); //Defines position to write from first row/first column 
lcd.print ("Menu Demo");
delay (5000); //Wait 5 seconds 
lcd.clear(); //Clear the whole LCD

}

void loop() {

  if (refreshLCD == true) // If were allowed to update LCD 
  


}
