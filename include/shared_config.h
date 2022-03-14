#include <string.h>

// Radio
#define RFM95_CS 10         // chip select pin
#define RFM95_RST 4         // reset pin
#define RFM95_INT 3         // interupt pin
#define RF95_FREQ 915.0     // Change to 434.0 or other frequency, must match RX's freq!
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Serial
#define BAUD_RATE 9600

// heartbeat
long heartbeatPreviousTime = 0;
unsigned short int heartbeatInterval = 5000;    // in msec