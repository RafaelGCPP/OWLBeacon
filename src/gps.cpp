#include <HardwareSerial.h>
#include <MicroNMEA.h>
#include "pins.h"
#include "gps.h"

HardwareSerial gps_serial(1);
char buffer[256];
MicroNMEA gps_data(buffer,sizeof(buffer));

void pps_interrupt() {

}

void setup_gps() {
  gps_serial.setPins(PIN_UART1_RXD,PIN_UART1_TXD);
  pinMode(PIN_GPS_PPS, INPUT);
  attachInterrupt(PIN_GPS_PPS,pps_interrupt,RISING);
}

void  query_gps() {

  while(gps_serial.available()) {
    char c=gps_serial.read();
    if (gps_data.process(c)) {

    }
  }
    
}