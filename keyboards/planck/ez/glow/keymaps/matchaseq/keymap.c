#include QMK_KEYBOARD_H
#include "eeprom.h"
#include "matchaseq.h"
#include "constants.h"
#include "symbols.h"
#include "utilities.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [SEQ] = LAYOUT_ortho_4x12( \
  SEQ_A1, SEQ_A2,   SEQ_A3,  SEQ_A4,  SEQ_MUTEA, SEQ_BANK,     SEQ_SEL,  SEQ_MUTEB, SEQ_B1,  SEQ_B2,  SEQ_B3,  SEQ_B4,  \
  SEQ_A5, SEQ_A6,   SEQ_A7,  SEQ_A8,  SEQ_DIV,   SEQ_VEL,      SEQ_MODE, SEQ_PAGE,  SEQ_B5,  SEQ_B6,  SEQ_B7,  SEQ_B8,  \
  SEQ_A9, SEQ_A10,  SEQ_A11, SEQ_A12, DEBUG,     RESET,        KC_NO,    SEQ_COPY,  SEQ_B9,  SEQ_B10, SEQ_B11, SEQ_B12, \
  SEQ_A13, SEQ_A14, SEQ_A15, SEQ_A16, SEQ_LAST,  SEQ_PLAYSTOP,           SEQ_BPM,   SEQ_B13, SEQ_B14, SEQ_B15, SEQ_B16  \
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {
        case SEQ_A1 ... SEQ_A16:
        case SEQ_B1 ... SEQ_B16:
            toggle_step(keycode);
        break;
        CASE SEQ_PLAYSTOP:
            toggle_play();
        break;
    }
    return true;
}
