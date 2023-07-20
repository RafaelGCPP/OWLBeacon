#include "state.h"
#include "display.h"
#include "rfgen.h"

TransceiverState current_state;

void set_state(TransceiverState new_state)
{
    if (current_state == new_state)
        return;

    switch (new_state)
    {
    case STATE_PPS_UPDATE:
        update_display(current_state);
        return;

    case STATE_TRANSMITTING:
        start_transmitting();
        break;

    case STATE_FIX_LOST:
        if (current_state==STATE_TRANSMITTING)
            return; // wait for the transmission to finish
        break;

    default:
        break;
    }

    current_state = new_state;
    update_display(current_state);
}
