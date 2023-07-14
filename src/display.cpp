#include "display.h"
#include "state.h"
#include "gps.h"
#include "config.h"
#include "pins.h"

enum TransceiverState previous_state;

TFT_eSPI tft = TFT_eSPI();

void setup_display()
{
    previous_state = STATE_UNDEFINED;

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    pinMode(PIN_LCD_POWER_ON, OUTPUT);
    digitalWrite(PIN_LCD_POWER_ON, LOW);
}

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

void update_display()
{
    TransceiverState current_state = get_state();

    if (previous_state == current_state)
        return;

    previous_state = current_state;

    switch (current_state)
    {
    case STATE_NO_FIX:
        tft.fillScreen(TFT_RED);
        tft.setTextColor(TFT_YELLOW, TFT_RED);
        tft.setCursor(5, 5, 4);
        tft.print("Waiting for GPS fix.");
        return;

    case STATE_READY:
        tft.fillScreen(TFT_DARKGREEN);
        tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
        break;

    case STATE_FIX_LOST:
        tft.fillScreen(TFT_RED);
        tft.setTextColor(TFT_YELLOW, TFT_RED);
        break;

    case STATE_TRANSMITTING:
        tft.fillScreen(TFT_BLUE);
        tft.setTextColor(TFT_YELLOW, TFT_BLUE);
        break;

    default:
        break;
    }
    char buffer[BUFFER_SIZE];
    tft.setCursor(5, 5, 4);

    long l = gps_data.getLatitude();
    milionths_to_string(l, buffer);

    tft.print("Lat: ");
    tft.print(buffer);
    tft.println((l > 0) ? " N" : " S");

    tft.setCursor(5, tft.getCursorY());

    l = gps_data.getLongitude();
    milionths_to_string(l, buffer);

    tft.print("Lon: ");
    tft.print(buffer);
    tft.println((l > 0) ? " E" : " W");
    tft.println();

    snprintf(buffer, BUFFER_SIZE, "%02d/%02d/%4d", gps_data.getDay(), gps_data.getMonth(), gps_data.getYear());
    tft.setCursor(5, tft.getCursorY());
    tft.println(buffer);

    tft.setCursor(5, tft.getCursorY());
    snprintf(buffer, BUFFER_SIZE, "%02d:%02d:%02d UTC", gps_data.getHour(), gps_data.getMinute(), gps_data.getSecond());
    tft.println(buffer);
}