#include <HardwareSerial.h>
#include <MicroNMEA.h>
#include "pins.h"
#include "gps.h"
#include "config.h"
#include "state.h"

HardwareSerial gps_serial(1);
char buffer[BUFFER_SIZE];
MicroNMEA gps_data(buffer, BUFFER_SIZE);


bool in_scheduled_time()
{
  return (gps_data.getMinute() % 2 == 0) && (gps_data.getSecond() >= 0) && (gps_data.getSecond() < 2);
}

// PPS interrupt handler
void IRAM_ATTR pps_interrupt()
{
  
  if (in_scheduled_time())
    set_state_from_isr(STATE_TRANSMITTING);
  else
    set_state_from_isr(STATE_PPS_UPDATE);
}

// GPS setup
// GPS is connected to UART1
// PPS is connected to PIN_GPS_PPS
void setup_gps()
{
  gps_serial.begin(GPS_BAUD_RATE, SERIAL_8N1, PIN_UART1_RXD, PIN_UART1_TXD);
  pinMode(PIN_GPS_PPS, INPUT);
  attachInterrupt(PIN_GPS_PPS, pps_interrupt, RISING);
}

// Convert millionths of degrees to degrees, minutes, seconds
// The output is in the format DD`MM'SS.SS"
// where DD is degrees, MM is minutes, SS.SS is seconds
//
void milionths_to_string(long value, char *out)
{
  ldiv_t result;
  int degrees, minutes;
  float seconds;

  result = ldiv(abs(value), 1000000L);
  degrees = result.quot;
  value = result.rem * 60;

  result = ldiv(value, 1000000L);
  minutes = result.quot;

  value = result.rem * 60;
  seconds = value / 1.0e6;

  snprintf(out, BUFFER_SIZE, "%d`%d'%5.2f\"", degrees, minutes, seconds);
}

// Convert millionths of degrees to Maidenhead locator
void maidenhead(long lat, long lon, int size, char *locator)
{
  ldiv_t dlon;
  ldiv_t dlat;

  lon += 180000000; // GPS returns millionths of degrees
  lat += 90000000;

  locator[0] = '\0';
  if (size >= 4)
  {
    // calculate field: there are 18 fields in total (A to R), each 20 degrees wide
    dlon = ldiv(lon, 20000000);
    dlat = ldiv(lat, 10000000);

    locator[0] = (char)(((int)(dlon.quot)) + 'A');
    locator[1] = (char)(((int)(dlat.quot)) + 'A');

    lon = dlon.rem;
    lat = dlat.rem;

    // calculate square: there are 10 squares in each field (0 to 9)
    // Taking the longitude as example (the latitute is analogous):
    // the remainder is 20.000.000ths of a field. Thus, the square is
    // 10 * rem / 20.000.000 = rem / 2.000.000

    dlon = ldiv(lon, 2000000);
    dlat = ldiv(lat, 1000000);

    locator[2] = (char)(((int)(dlon.quot)) + '0');
    locator[3] = (char)(((int)(dlat.quot)) + '0');
    locator[4] = '\0';
  }

  if (size >= 6)
  {
    // calculate subsquare: there are 24 subsquares in each square
    // Taking the longitude as example (the latitute is analogous):
    // the remainder is 2.000.000ths of a square. Thus, the square is
    // 24 * rem / 2.000.000  = 12 * rem / 1.000.000 = 3 * rem / 250.000
    // For the latitude we have
    // 24 * rem / 1.000.000 = 3 * rem / 125.000

    lon = dlon.rem;
    lat = dlat.rem;

    dlon = ldiv(3 * lon, 250000);
    dlat = ldiv(3 * lat, 125000);
    locator[4] = (char)(((int)(dlon.quot)) + 'a');
    locator[5] = (char)(((int)(dlat.quot)) + 'a');
    locator[6] = '\0';
  }

  if (size >= 8)
  {
    // calculate extended square: there are 10 extended squares in each subsquare
    // Taking the longitude as example (the latitute is analogous):
    // the remainder is 250.000th of a subsquare. Thus, the extended square is
    // 10 * rem / 250.000 = rem / 25.000
    // For the latitude we have
    // 10 * rem / 125.000 =  rem / 12.500

    lon = dlon.rem;
    lat = dlat.rem;

    dlon = ldiv(lon, 25000);
    dlat = ldiv(lat, 12500);
    locator[6] = (char)(((int)(dlon.quot)) + '0');
    locator[7] = (char)(((int)(dlat.quot)) + '0');
    locator[8] = '\0';
  }

  if (size >= 10)
  {
    // calculate extended square: there are 24 extended squares in each subsquare
    // Taking the longitude as example (the latitute is analogous):
    // the remainder is 25.000th of a subsquare. Thus, the extended square is
    // 24 * rem / 25.000 = rem   * 3 / 3125
    // For the latitude we have
    // 24 * rem / 12.5000 =  rem * 6/ 3125

    lon = dlon.rem;
    lat = dlat.rem;

    dlon = ldiv(lon * 3, 3125);
    dlat = ldiv(lat * 6, 3125);
    locator[8] = (char)(((int)(dlon.quot)) + 'a');
    locator[9] = (char)(((int)(dlat.quot)) + 'a');
    locator[10] = '\0';
  }
}

long get_latitude_long()
{
  return gps_data.getLatitude();
}
long get_longitude_long()
{
  return gps_data.getLongitude();
}
// Get latitude in degrees, minutes, seconds
void get_latitude_text(char *buffer, size_t buffer_size)
{
  long l = gps_data.getLatitude();
  milionths_to_string(l, buffer);
  strncat(buffer, (l > 0) ? " N" : " S", buffer_size);
}
// Get longitude in degrees, minutes, seconds
void get_longitude_text(char *buffer, size_t buffer_size)
{
  long l = gps_data.getLongitude();
  milionths_to_string(l, buffer);
  strncat(buffer, (l > 0) ? " E" : " W", buffer_size);
}

// Get Maidenhead locator
void get_maidenhead_text(char *buffer, int size)
{
  long lat = gps_data.getLatitude();
  long lon = gps_data.getLongitude();
  maidenhead(lat, lon, size, buffer);
}

// Get date and time
void get_date_text(char *buffer, size_t buffer_size)
{
  snprintf(buffer, buffer_size, "%02d/%02d/%02d", gps_data.getDay(), gps_data.getMonth(), gps_data.getYear());
}

void get_time_text(char *buffer, size_t buffer_size)
{
  snprintf(buffer, buffer_size, "%02d:%02d:%02d UTC", gps_data.getHour(), gps_data.getMinute(), gps_data.getSecond());
}

// This function is called from the main loop to query the GPS
void query_gps()
{

  while (gps_serial.available())
  {
    char c = gps_serial.read();
    if (!gps_data.process(c))
      return;
    if (!gps_data.isValid())
    {
      set_state(STATE_FIX_LOST);
      return;
    };
    set_state(STATE_READY);
  }
}