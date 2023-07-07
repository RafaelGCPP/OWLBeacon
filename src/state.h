#pragma once

enum TransceiverState {
    STATE_NO_FIX,
    STATE_READY,
    STATE_TRANSMITTING
};

extern TransceiverState current_state;