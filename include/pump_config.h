#include <shared_config.h>
#include <math.h>

// RF95 Radio I/O definitions
#define RFM95_CS 10         // chip select pin
#define RFM95_RST 4         // reset pin
#define RFM95_INT 3         // interupt pin

// Radio settings
long radioPreviousTime = 0;
unsigned short int radioStatusTransmitInterval = 4000;   // interval in msecs

// fuel sensor
#define FUEL_SENSOR_PIN 0       // analog input