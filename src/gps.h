#pragma once
#include <HardwareSerial.h>
#include <MicroNMEA.h>

extern HardwareSerial gps_serial;

void pps_interrupt();
void setup_gps();
void query_gps();

long get_latitude_long();
long get_longitude_long();
void get_latitude_text(char *buffer, size_t buffer_size);
void get_longitude_text(char *buffer, size_t buffer_size);
void get_date_text(char *buffer, size_t buffer_size);
void get_time_text(char *buffer, size_t buffer_size);
void get_maidenhead_text(char *buffer, int size);