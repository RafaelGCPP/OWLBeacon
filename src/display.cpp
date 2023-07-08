#include "display.h"
#include "state.h"
#include "gps.h"
#include "config.h"

enum TransceiverState previous_state;

TFT_eSPI tft = TFT_eSPI();

void setup_display()
{
    previous_state = STATE_UNDEFINED;
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void milionths_to_string(long value, char *out) 
{
    ldiv_t result;
    int degrees, minutes;
    float seconds;

    result=ldiv(abs(value),1000000L);
    value=result.rem*60;

    result=ldiv(value,1000000L);
    minutes=result.quot;

    value=result.rem*60;
    seconds=value/1.0e6;

    snprintf(out,BUFFER_SIZE,"%d`%d'%5.2f\"",degrees,minutes,seconds);
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

  
    long l=gps_data.getLatitude();
    milionths_to_string(l,buffer);

    tft.print("Lat: ");
    tft.print(buffer);
    tft.println((l>0)?" N":" S");


    l=gps_data.getLongitude();
    milionths_to_string(l,buffer);

    tft.print("Lon: ");
    tft.print(buffer);
    tft.println((l>0)?" E":" W");

}