#include <shared_config.h>
#include <math.h>

// RF95 Radio I/O definitions
#define RFM95_CS 53         // chip select pin
#define RFM95_RST 4         // reset pin
#define RFM95_INT 2         // interupt pin

// Radio settings
long radioPreviousTime = 0;
unsigned short int radioStatusTransmitInterval = 4000;   // interval in msecs

// fuel sensor
#define FUEL_SENSOR_PIN 0       // analog input

// servo
#define DYNAMIXEL_CONTROL_PIN 6
#define DYNAMIXEL_BAUDRATE 1000000

// killswitch
#define RELAY_PIN 38

// rpm sensor
#define RPM_SENSOR_PIN 37