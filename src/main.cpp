#include <Arduino.h>
#include <HardwareSerial.h>
#include "gps.h"
#include "display.h"
#include "pins.h"
#include "config.h"
#include "state.h"
#include "rfgen.h"
#include <Preferences.h>

HWCDC console=Serial;
Preferences preferences;


void get_preferences() 
{
  //get preferences
  preferences.begin("owlbeacon", false);
  si5351_calibration=preferences.getInt("si5351_calibration", 0);
  preferences.getString("callsign", callsign, 7);
  preferences.end();
}

void save_preferences() 
{
  //save preferences
  preferences.begin("owlbeacon", false);
  preferences.putInt("si5351_calibration", si5351_calibration);
  preferences.putString("callsign", callsign);
  preferences.end();
}

void setup() {

  //setup console
  console.begin(CONSOLE_BAUD_RATE);
  
  //setup initial state
  set_state(STATE_NO_FIX);

  //setup peripherals
  setup_gps();
  setup_display();
  setup_rfgen();

}

void loop() {
  update_display();  
  query_gps();
}

