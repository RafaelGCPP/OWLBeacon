// include config state and pins
#include "config.h"
#include "pins.h"
#include "state.h"
#include "rfgen.h"
#include "gps.h"
#include <Wire.h>
#include <si5351.h>
#include <JTEncode.h>

Si5351 rfgen;
int32_t si5351_calibration = 0;
char callsign[7] = "PU1OWL";
int dbm = 0;
unsigned long long frequency = 2810000000ULL;

void setup_rfgen()
{
    // setup Wire IIC pins
    Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);

    // setup Si5351
    rfgen.init(SI5351_CRYSTAL_LOAD_8PF, 25000000, si5351_calibration);

    // set drive strength on outputs 0,1,2   to 8mA
    rfgen.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    rfgen.drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA);
    rfgen.drive_strength(SI5351_CLK2, SI5351_DRIVE_8MA);

    // disable all outputs
    rfgen.output_enable(SI5351_CLK0, 0);
    rfgen.output_enable(SI5351_CLK1, 0);
    rfgen.output_enable(SI5351_CLK2, 0);
}

void wspr_beacon_thread()
{
    JTEncode jtencode;
    char locator[5];
    uint8_t tx_buffer[WSPR_SYMBOL_COUNT];

    // get current GPS location
    get_maidenhead_text(locator, 4);

    // clear tx_buffer
    memset(tx_buffer, 0, sizeof(tx_buffer));

    // encode WSPR message
    jtencode.wspr_encode(callsign, locator, dbm, tx_buffer);

    // set frequency
    rfgen.set_freq(frequency, SI5351_CLK0);
    rfgen.output_enable(SI5351_CLK0, 1);
    // Now transmit the channel symbols
    for (int i = 0; i < WSPR_SYMBOL_COUNT; i++)
    {
        rfgen.set_freq((frequency * 100) + (tx_buffer[i] * WSPR_TONE_SPACING), SI5351_CLK0);
        delay(WSPR_TONE_DELAY);
    }
    rfgen.output_enable(SI5351_CLK0, 0);
}