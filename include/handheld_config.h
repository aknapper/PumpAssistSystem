#include <shared_config.h>

// LCD and button dependencies
#include <Wire.h>               // deals with I2C connections
#include <LiquidCrystal_I2C.h>  // activates the LCD I2C library
#include <Arduino.h>

// RF95 Radio I/O definitions
#define RFM95_CS 53         // chip select pin
#define RFM95_RST 4         // reset pin
#define RFM95_INT 2         // interupt pin

// Radio settings
long radioPreviousTime = 0;
unsigned short int radioStatusTransmitInterval = 4000;   // interval in msecs

// Digital pins - Buttons
const int leftButtonPin = 6;
const int rightButtonPin = 8;
const int upButtonPin = 10;
const int downButtonPin = 12;