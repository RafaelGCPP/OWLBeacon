#include "state.h"
#include "display.h"
#include "rfgen.h"

TransceiverState current_state;
portMUX_TYPE state_spinlock = portMUX_INITIALIZER_UNLOCKED;

void set_state_internal(TransceiverState new_state)
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
        if (current_state == STATE_TRANSMITTING)
            return; // wait for the transmission to finish
        break;

    default:
        break;
    }

    current_state = new_state;
    update_display(current_state);
}

void set_state_from_isr(TransceiverState new_state)
{
    taskENTER_CRITICAL_ISR(&state_spinlock);
    set_state_internal(new_state);
    taskEXIT_CRITICAL_ISR(&state_spinlock);
}

void set_state(TransceiverState new_state)
{
    taskENTER_CRITICAL(&state_spinlock);
    set_state_internal(new_state);
    taskEXIT_CRITICAL(&state_spinlock);
}
