//include config state and pins
#include "config.h"
#include "pins.h"
#include "state.h"
#include "rfgen.h"
#include <Wire.h>
#include <si5351.h>
#include <JTEncode.h>

Si5351  rfgen;
int32_t si5351_calibration = 0;
char callsign[7]="PU1OWL";

void setup_rfgen()
{
    //setup Wire IIC pins
    Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
    
    //setup Si5351
    rfgen.init(SI5351_CRYSTAL_LOAD_8PF, 25000000, si5351_calibration);

    //set drive strength on outputs 0,1,2   to 8mA
    rfgen.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    rfgen.drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA);
    rfgen.drive_strength(SI5351_CLK2, SI5351_DRIVE_8MA);
    
    //disable all outputs
    rfgen.output_enable(SI5351_CLK0, 0);
    rfgen.output_enable(SI5351_CLK1, 0);
    rfgen.output_enable(SI5351_CLK2, 0);
}

