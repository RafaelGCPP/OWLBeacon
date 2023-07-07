#include "state.h"

TransceiverState current_state;


void set_state(TransceiverState new_state) {
    current_state=new_state;
}

TransceiverState get_state() {
    return current_state;
}