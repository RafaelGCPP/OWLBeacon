#pragma once
#include <inttypes.h>

#define WSPR_TONE_SPACING       146
#define WSPR_TONE_DELAY         683

extern int32_t si5351_calibration;
extern char callsign[7];
extern int dbm;
extern unsigned long long frequency;

void setup_rfgen();
void start_transmitting();