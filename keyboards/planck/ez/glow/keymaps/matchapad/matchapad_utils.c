#include QMK_KEYBOARD_H
#include "matchapad_types.h"
#include "matchapad_seq.h"
extern matchapad_state_t matchapad_state;
extern matchapad_internal_state_t matchapad_internal_state;

void matchapad_state_dump(void) {
  uprintln("*** matchapad state ***");
  uprintf("clock mode: %d\n", matchapad_state.midi_mode);
  uprintf("active mode: %d\n", matchapad_state.active_mode);
}

void matchapad_internal_state_dump(void) {
  int i;
  uprintln("*** matchapad internal state ***");
  uprintf("load_sysex? %d\n", matchapad_internal_state.load_sysex);
  uprintf("sysex_guard: 0x");
  for (i = 0; i < SYSEX_BUF_SIZE; i++) {
    uprintf("%02X", matchapad_internal_state.sysex_guard[i]);
  }
  uprintln(".");
  uprintf("Current step: %d\n", matchapad_internal_state.current_step);
}

void matchapad_switch_mode(matchapad_mode mode) {
  matchapad_state.active_mode = mode;
  switch (mode) {
    case MODE_SEQ8:
      uprintln("matchapad_switch_mode: seq8");
      matchapad_seq8_init();
    break;
    default:
      uprintln("mode not implemented yet");
  }
}
