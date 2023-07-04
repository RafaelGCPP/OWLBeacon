#include "display.h"

TFT_eSPI tft = TFT_eSPI(); 

void setup_display() {
    
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void update_display() {
    tft.fillScreen(TFT_RED); delay(1000);
    tft.fillScreen(TFT_GREEN); delay(1000);
    tft.fillScreen(TFT_BLUE); delay(1000);
}