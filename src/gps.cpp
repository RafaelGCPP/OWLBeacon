#include <HardwareSerial.h>
#include <UbxGpsNavPvt.h>
#include "pins.h"
#include "gps.h"

HardwareSerial gps_serial(1);
UbxGpsNavPvt<HardwareSerial> gps(gps_serial);

void pps_interrupt() {

}

void setup_gps() {
  gps_serial.setPins(PIN_UART1_RXD,PIN_UART1_TXD);
  gps.begin(115200); 
  pinMode(PIN_GPS_PPS, INPUT);
  attachInterrupt(PIN_GPS_PPS,pps_interrupt,RISING);
}

void  query_gps() {
    
}