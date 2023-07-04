#include <Arduino.h>
#include <HardwareSerial.h>

#include "display.h"
#include "pins.h"

HWCDC console=Serial;
HardwareSerial gps_serial(1);

void setup() {

  console.begin(115200);
  gps_serial.begin(115200,SERIAL_8N1,PIN_UART1_RXD,PIN_UART1_TXD);

  pinMode(PIN_LCD_POWER_ON, OUTPUT);
  digitalWrite(PIN_LCD_POWER_ON, HIGH);


  setup_display();


}

void loop() {
  update_display();

}

