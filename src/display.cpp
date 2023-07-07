#include "display.h"

TFT_eSPI tft = TFT_eSPI(); 

void setup_display() {
    
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void update_display() {

}