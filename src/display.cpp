#include "display.h"
#include "state.h"

enum TransceiverState previous_state;

TFT_eSPI tft = TFT_eSPI(); 

void setup_display() {
    previous_state=STATE_UNDEFINED;
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void update_display() {
    TransceiverState current_state=get_state();

    if (previous_state==current_state) return;

    previous_state=current_state;

    switch (current_state)
    {
    case STATE_NO_FIX:
        tft.fillScreen(TFT_RED);
        tft.setTextColor(TFT_YELLOW,TFT_RED);
        tft.setCursor(5,5,4);
        tft.print("Waiting for GPS fix.");
        break;
    
    case STATE_READY:
        tft.fillScreen(TFT_DARKGREEN);
        tft.setTextColor(TFT_WHITE,TFT_DARKGREEN);
        tft.setCursor(5,5,4);
        tft.print("GPS fix acquired.");
        break; 
    
    default:
        break;
    }
}