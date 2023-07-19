#include <Arduino.h>
#include <HardwareSerial.h>
#include "gps.h"
#include "display.h"
#include "pins.h"
#include "config.h"
#include "state.h"
#include "rfgen.h"
#include <Preferences.h>

HWCDC console = Serial;
Preferences preferences;

void get_preferences()
{
  // get preferences
  preferences.begin("owlbeacon", false);
  si5351_calibration = preferences.getInt("si5351_calibration", 0);
  preferences.getString("callsign", callsign, 7);
  frequency = preferences.getULong64("frequency", 2810000000ULL);
  dbm = preferences.getInt("dbm", 0);
  preferences.end();
}

void save_preferences()
{
  // save preferences
  preferences.begin("owlbeacon", false);
  preferences.putInt("si5351_calibration", si5351_calibration);
  preferences.putString("callsign", callsign);
  preferences.putULong64("frequency", frequency);
  preferences.putInt("dbm", dbm);
  preferences.end();
}

void setup()
{

  // setup console
  console.begin(CONSOLE_BAUD_RATE);

  // setup initial state
  set_state(STATE_NO_FIX);

  // setup peripherals
  setup_gps();
  setup_display();
  setup_rfgen();
  update_display();
}

void loop()
{
  TransceiverState previous_state = get_state();

  query_gps();

  switch (get_state())
    {
    case STATE_PPS_UPDATE:
      update_display();
      set_state(STATE_READY);
      break;
    case STATE_TRANSMITTING:
      if (previous_state != STATE_TRANSMITTING)
      {
        start_transmitting();
      }
      break;
    case STATE_FIX_LOST:
      if (previous_state != STATE_FIX_LOST)
      {
        update_display();
      }
      break;

    default:
      break;
    }
}
