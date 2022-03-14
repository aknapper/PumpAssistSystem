#include <shared_config.h>
#include <math.h>

// RF95 Radio I/O definitions

// Radio settings
long radioPreviousTime = 0;
unsigned short int radioStatusTransmitInterval = 4000;   // interval in msecs