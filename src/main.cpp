#include <Arduino.h>
#include "display.h"
#include "pins.h"

void setup() {
  Serial.begin(115200);

  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);


  setup_display();


}

void loop() {
  update_display();

}

