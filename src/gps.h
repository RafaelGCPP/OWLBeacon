#pragma once
#include <UbxGpsNavPvt.h>

extern UbxGpsNavPvt<HardwareSerial> gps;

void pps_interrupt();
void setup_gps();
void query_gps();