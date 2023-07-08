#include <HardwareSerial.h>
#include <MicroNMEA.h>
#include "pins.h"
#include "gps.h"
#include "config.h"
#include "state.h"

HardwareSerial gps_serial(1);
char buffer[BUFFER_SIZE];
MicroNMEA gps_data(buffer,BUFFER_SIZE);

void IRAM_ATTR pps_interrupt() {
  if (get_state()==STATE_READY) set_state(STATE_PPS_UPDATE);  
}

void setup_gps() {
  gps_serial.begin(GPS_BAUD_RATE,SERIAL_8N1,PIN_UART1_RXD, PIN_UART1_TXD);
  pinMode(PIN_GPS_PPS, INPUT);
  attachInterrupt(PIN_GPS_PPS,pps_interrupt,RISING);
}

void  query_gps() {

  while(gps_serial.available()) {
    char c=gps_serial.read();
    if (!gps_data.process(c)) return;
    if (!gps_data.isValid()) {
      if (get_state()==STATE_READY) set_state(STATE_FIX_LOST);
      return;
    };
    set_state(STATE_READY);
  }
    
}