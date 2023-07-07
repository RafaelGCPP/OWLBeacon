#include <Arduino.h>
#include <HardwareSerial.h>
#include "gps.h"
#include "display.h"
#include "pins.h"
#include "config.h"
#include "state.h"

HWCDC console=Serial;
TransceiverState current_state;

void setup() {

  console.begin(CONSOLE_BAUD_RATE);
  
  current_state=STATE_NO_FIX;

  pinMode(PIN_LCD_POWER_ON, OUTPUT);
  digitalWrite(PIN_LCD_POWER_ON, LOW);

  setup_gps();

  setup_display();

  console.print("Teste");

}

void loop() {
  query_gps();
  update_display();  
}

