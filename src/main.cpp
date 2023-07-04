#include <Arduino.h>
#include "tft_config.h"
#include <TFT_eSPI.h> 
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); 

void setup() {
    Serial.begin(115200);

    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);


    tft.fillScreen(TFT_RED); delay(1000);
    tft.fillScreen(TFT_GREEN); delay(1000);
    tft.fillScreen(TFT_BLUE); delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
}

