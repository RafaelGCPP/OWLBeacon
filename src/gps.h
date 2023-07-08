#pragma once
#include <HardwareSerial.h>
#include <MicroNMEA.h>

extern HardwareSerial gps_serial;
extern MicroNMEA gps_data;

void pps_interrupt();
void setup_gps();
void query_gps();