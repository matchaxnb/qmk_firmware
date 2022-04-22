/**
  This defines the base stuff for the matcha pad
**/
#include QMK_KEYBOARD_H
#include "matchapad_types.h"

matchapad_state_t matchapad_state = {
  .midi_mode = MODE_UNSET,
};

matchapad_internal_state_t matchapad_internal_state = {
  .load_sysex = 0,
};
