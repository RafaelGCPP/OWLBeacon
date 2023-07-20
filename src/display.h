#pragma once
#include <TFT_eSPI.h>
#include "state.h"

void setup_display();
void update_display(TransceiverState current_state);
extern TFT_eSPI tft ;
