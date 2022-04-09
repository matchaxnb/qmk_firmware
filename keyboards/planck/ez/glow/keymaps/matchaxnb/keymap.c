#include QMK_KEYBOARD_H
#include "eeprom.h"
#include "keymap_us_international.h"

enum planck_keycodes {
  RGB_SLD = EZ_SAFE_RANGE,
};

enum planck_layers {
  _BASE
};

enum seq_instruments {
  INST1,
  INST2,
  INST3,
  INST4,
  INST5,
  INST6
};

const uint16_t seq_instruments_notes[SEQUENCER_TRACKS] = {
  [INST1] = MI_C_1, [INST2] = MI_D_1,
  [INST3] = MI_Fs_1, [INST4] = MI_As_1,
  [INST5] = MI_Cs_2, [INST6] = MI_Ds_2
};
/* planck grid
  0  1  2  3  4  5  6  7  8  9  10 11
  12 13 14 15 16 17 18 19 20 21 22 23
  24 25 26 27 28 29 30 31 32 33 34 35
  36 37 38 39 40 41 41 42 43 44 45 46
*/
const uint8_t PROGMEM stepsmap[DRIVER_LED_TOTAL] = {
   1, 13, 25, 37, // col1
   2, 14, 26, 38, // col2
   3, 15, 27, 39, // col3
   4, 16, 28, 40, // col4
   7, 19, 31, 42, // col5
   8, 20, 32, 43, // col6
   9, 21, 33, 44, // col7
  10, 22, 34, 45 // col8
};

// add a RESET key on the trident so I can dev
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_planck_grid(
    SQ_SALL,        SQ_S(0),     SQ_S(4),  SQ_S( 8),    SQ_S(12),    SQ_T(INST1),   SQ_T(INST4),  SQ_S(16), SQ_S(20), SQ_S(24), SQ_S(28), SQ_TMPD,
    SQ_SCLR,        SQ_S(1),     SQ_S(5),  SQ_S( 9),    SQ_S(13),    SQ_T(INST2),   SQ_T(INST5),  SQ_S(17), SQ_S(21), SQ_S(25), SQ_S(29), SQ_TMPU,
    DEBUG,          SQ_S(2),     SQ_S(6),  SQ_S(10),    SQ_S(14),    SQ_T(INST3),   SQ_T(INST6),  SQ_S(18), SQ_S(22), SQ_S(26), SQ_S(30), SQ_RESD,
    RESET,          SQ_S(3),     SQ_S(7),  SQ_S(11),    SQ_S(15),    SQ_TOG,              KC_NO,  SQ_S(19), SQ_S(23), SQ_S(27), SQ_S(31), SQ_RESU
  ),
};
extern rgb_config_t rgb_matrix_config;

uint8_t active_step;

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
  sequencer_set_tempo(120);
  sequencer_set_track_notes(seq_instruments_notes);
  active_step = SEQUENCER_STEPS + 1;
}

#define L_YELW {179, 176, 104}
#define L_BLUE {32, 255, 234}
#define L_AQUA {12, 225, 241}
#define L_SKY  {0, 204, 255}
#define L_PURP {169, 120, 255}
#define L_DARK {0, 0, 0}

const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
    [0] = {
            L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE,
            L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE, L_BLUE,
            L_DARK, L_DARK, L_DARK, L_DARK, L_DARK, L_DARK, L_DARK, L_DARK, L_DARK, L_DARK, L_DARK, L_DARK,
            L_DARK, L_DARK, L_DARK,  L_DARK, L_DARK, L_DARK /*spc*/, L_DARK, L_DARK, L_DARK, L_DARK, L_DARK,
            },
};



/*bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // no special things now
  return true;
}*/

extern rgb_config_t rgb_matrix_config;

void rgb_matrix_indicators_user(void) {
    uint8_t i;
    bool is_current_step;
    bool step_is_on;
    bool sequencer_active = is_sequencer_on();
    float f = (float) rgb_matrix_config.hsv.v / UINT8_MAX;
    uint8_t cur_step = sequencer_get_current_step();
    rgb_matrix_set_color_all(f * cur_step * 3 + 1, f * cur_step * 3 + 1, f * cur_step * 3 + 1);
    if (active_step != cur_step) {
    
      for (i=0; i < SEQUENCER_STEPS; i++) {
          is_current_step = sequencer_active && cur_step == i;
          step_is_on = is_sequencer_step_on(i);
          if (is_current_step && step_is_on) {
              rgb_matrix_set_color(stepsmap[i], f * 255, f * 0, f * 0);
          } else if (is_current_step && !step_is_on) {
      rgb_matrix_set_color(stepsmap[i], f * 255, f * 255, 0);
          } else if (!is_current_step && step_is_on) {
              rgb_matrix_set_color(stepsmap[i], f * 10, f * 0, f * 0);
          } 
      } 
    } else {
      active_step = cur_step;
    }
}
