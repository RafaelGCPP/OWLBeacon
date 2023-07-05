#include <Arduino.h>
#include <HardwareSerial.h>
#include <UbxGpsNavPvt.h>
#include "gps.h"
#include "display.h"
#include "pins.h"

HWCDC console=Serial;


void setup() {

  console.begin(115200);

  

  pinMode(PIN_LCD_POWER_ON, OUTPUT);
  digitalWrite(PIN_LCD_POWER_ON, HIGH);

  setup_gps();

  setup_display();


}

void loop() {
  query_gps();
  update_display();
  
}

