#include <handheld_io.h>
#include <RH_RF95.h>
#include <HPP_Controller.h>

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
HPP_Controller pump_controller(1,2,3)

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}