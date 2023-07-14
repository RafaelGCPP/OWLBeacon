#include <Arduino.h>
#include <HardwareSerial.h>
#include "gps.h"
#include "display.h"
#include "pins.h"
#include "config.h"
#include "state.h"
#include "rfgen.h"

HWCDC console=Serial;

void setup() {

  console.begin(CONSOLE_BAUD_RATE);
  
  set_state(STATE_NO_FIX);

  pinMode(PIN_LCD_POWER_ON, OUTPUT);
  digitalWrite(PIN_LCD_POWER_ON, LOW);

  setup_gps();

  setup_display();

  setup_rfgen();

  console.print("Teste");

}

void loop() {
  update_display();  
  query_gps();

}

