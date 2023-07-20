#pragma once

enum TransceiverState {
    STATE_NO_FIX,
    STATE_READY,
    STATE_PPS_UPDATE,
    STATE_TRANSMITTING,
    STATE_FIX_LOST,
    STATE_UNDEFINED=-1
};

// extern TransceiverState current_state;
void set_state(TransceiverState);
