#include QMK_KEYBOARD_H
#include "eeprom.h"
#include "matchapad_utils.h"
#include "matchapad_midi.h"
#include "matchapad_seq.h"
#include "keymap_us_international.h"

enum planck_keycodes {
  RGB_SLD = EZ_SAFE_RANGE,
};

enum planck_layers {
  _BASE
};


extern rgb_config_t rgb_matrix_config;
extern rgb_config_t rgb_matrix_config;
extern sequencer_config_t sequencer_config;
extern MidiDevice midi_device;
// add a RESET key on the trident so I can dev
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_planck_grid(
    KC_A, KC_C, RESET, RESET, RESET, DEBUG, DEBUG, RESET, RESET, RESET, RESET, KC_NO,
    KC_B, KC_D, RESET, RESET, RESET, DEBUG, DEBUG, RESET, RESET, RESET, RESET, KC_NO,
    DEBUG,   KC_E, RESET, RESET, RESET, DEBUG, DEBUG, RESET, RESET, RESET, RESET, KC_NO,
    RESET,   RESET, RESET, RESET, RESET, KC_NO,            KC_NO, RESET, RESET, RESET, RESET, KC_NO
  ),
};


/* putting that into a struct */


void keyboard_post_init_user(void) {
  rgb_matrix_enable();
  matchapad_init_midi();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!record->event.pressed) {
    return true;
  }
  switch (keycode) {
    case DEBUG:
      uprintln ("debug");
      matchapad_state_dump();
      matchapad_internal_state_dump();
      matchapad_seq8_debug();
      return true;
      break;
    case KC_A:
      uprintln ("save config with sysex");
      save_config_with_sysex();
      return false;
      break;
    case KC_B:
      uprintln ("load config from sysex");
      load_config_from_sysex();
      return false;
    case KC_C:
      uprintln ("load test program");
      matchapad_switch_mode(MODE_SEQ8);
      matchapad_seq8_load_test_program();
      return false;
    case KC_D:
      matchapad_seq8_show_step(matchapad_seq8_next_step());
      matchapad_internal_state_dump();
      return false;
    case KC_E:
      matchapad_seq8_play_next_step();
      matchapad_internal_state_dump();
      return false;
  }
  return true;
}
