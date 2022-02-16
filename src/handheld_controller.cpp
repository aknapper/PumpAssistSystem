#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <SPI.h>

#include <HPP_Controller.h>
#include <handheld_config.h>

RH_RF95 driver(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(driver, SERVER_ADDRESS);

HPP_Controller handheld_controller(1,2,3);

void setup() {


}



void loop() {
  // put your main code here, to run repeatedly:
}