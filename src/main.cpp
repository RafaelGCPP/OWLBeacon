#include <Arduino.h>
#include <HardwareSerial.h>
#include <UbxGpsNavPvt.h>
#include "gps.h"
#include "display.h"
#include "pins.h"

HWCDC console=Serial;


void setup() {

  console.begin(115200);
  gps_serial.begin(115200,SERIAL_8N1,PIN_UART1_RXD, PIN_UART1_TXD);
  

  pinMode(PIN_LCD_POWER_ON, OUTPUT);
  digitalWrite(PIN_LCD_POWER_ON, LOW);

  // setup_gps();

  setup_display();

  console.print("Teste");

}

void loop() {
  query_gps();

  if (gps_serial.available()) {
    int buf=gps_serial.read();
    console.print((char) buf);
  }

  
}

