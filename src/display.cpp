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

void update_display(TransceiverState current_state)
{

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

    get_latitude_text(buffer, BUFFER_SIZE);
    tft.print("Lat: ");
    tft.println(buffer);

    get_longitude_text(buffer, BUFFER_SIZE);
    tft.setCursor(5, tft.getCursorY());
    tft.print("Lon: ");
    tft.println(buffer);

    get_maidenhead_text(buffer, BUFFER_SIZE);
    tft.setCursor(5, tft.getCursorY());
    tft.print("Locator: ");
    tft.println(buffer);

    get_date_text(buffer, BUFFER_SIZE);
    tft.setCursor(5, tft.getCursorY());
    tft.println(buffer);

    get_time_text(buffer, BUFFER_SIZE);
    tft.setCursor(5, tft.getCursorY());
    tft.println(buffer);
}