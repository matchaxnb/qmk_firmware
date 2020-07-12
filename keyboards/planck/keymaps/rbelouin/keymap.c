#include QMK_KEYBOARD_H
#include "muse.h"
#include "keymap_bepo.h"

enum planck_layers { _BASE, _LOWER, _RAISE, _ADJUST, _QWERTY, _SEQUENCER };

enum planck_sequencer_tracks {
    KICK,
    SNARE,
    HIHAT1,  // closed
    HIHAT2,  // open
    CRASH,
    RIDE
};

const uint16_t planck_sequencer_track_notes[SEQUENCER_TRACKS] = {[KICK] = MI_C_1, [SNARE] = MI_D_1, [HIHAT1] = MI_Fs_1, [HIHAT2] = MI_As_1, [CRASH] = MI_Cs_2, [RIDE] = MI_Ds_2};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define SEQUENCER TG(_SEQUENCER)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/**
 * Base: bépo layout’s three main rows.
 */
[_BASE] = LAYOUT_planck_grid(
    KC_TAB,     BP_B,     BP_ECUT,  BP_P,     BP_O,    BP_EGRV,   BP_DCRC,   BP_V,   BP_D,     BP_L,     BP_J,     BP_Z,
    KC_ESCAPE,  BP_A,     BP_U,     BP_I,     BP_E,    BP_COMM,   BP_C,      BP_T,   BP_S,     BP_R,     BP_N,     BP_M,
    BP_CCED,    BP_AGRV,  BP_Y,     BP_X,     BP_DOT,  BP_K,      BP_APOS,   BP_Q,   BP_G,     BP_H,     BP_F,     BP_W,
    KC_LSFT,    KC_LCTL,  KC_LOPT,  KC_LCMD,  LOWER,   KC_SPACE,  KC_SPACE,  RAISE,  KC_RCMD,  KC_ROPT,  KC_RCTL,  KC_RSFT
),

/**
 * Lower: bépo layout’s top row. With option pressed / with no modifiers / with shift pressed.
 * One exception though, the % and ` signs are more useful than a ° (shift =).
 */
[_LOWER] = LAYOUT_planck_grid(
    ROPT(BP_DLR),  ROPT(BP_DQOT),  ROPT(BP_LGIL),  ROPT(BP_RGIL),  ROPT(BP_LPRN),  ROPT(BP_RPRN),  LOPT(BP_AT),  LOPT(BP_PLUS),  LOPT(BP_MINS),  LOPT(BP_SLSH),  LOPT(BP_ASTR),  LOPT(BP_EQL),
    BP_DLR,        BP_DQOT,        BP_LGIL,        BP_RGIL,        BP_LPRN,        BP_RPRN,        BP_AT,        BP_PLUS,        BP_MINS,        BP_SLSH,        BP_ASTR,        BP_EQL,
    RSFT(BP_DLR),  RSFT(BP_DQOT),  RSFT(BP_LGIL),  RSFT(BP_RGIL),  RSFT(BP_LPRN),  RSFT(BP_RPRN),  LSFT(BP_AT),  LSFT(BP_PLUS),  LSFT(BP_MINS),  LSFT(BP_SLSH),  LSFT(BP_ASTR),  BP_PERC,
    _______,       _______,        _______,        _______,        _______,        KC_ENTER,       KC_ENTER,     _______,        _______,        _______,        _______,        _______
),

/**
 * Raise: custom coding layer.
 */
[_RAISE] = LAYOUT_planck_grid(
    _______,  ROPT(BP_B),  _______,  ROPT(BP_P),  BP_LESS,  _______,    _______,    BP_GRTR,  _______,  _______,  _______,        BP_EQL,
    BP_DLR,   BP_DQOT,     BP_LBRC,  BP_LCBR,     BP_LPRN,  _______,    _______,    BP_RPRN,  BP_RCBR,  BP_RBRC,  ROPT(BP_COMM),  BP_PERC,
    BP_HASH,  BP_1,        BP_2,     BP_3,        BP_4,     BP_5,       BP_6,       BP_7,     BP_8,     BP_9,     BP_0,           KC_CAPSLOCK,
    _______,  _______,     _______,  _______,     _______,  KC_BSPACE,  KC_BSPACE,  _______,  _______,  _______,  _______,        _______
),

/**
 * Adjust: media and navigation.
 */
[_ADJUST] = LAYOUT_planck_grid(
    RESET,      KC_MEDIA_PREV_TRACK,  KC_MEDIA_REWIND,    KC_MEDIA_FAST_FORWARD,  KC_MEDIA_NEXT_TRACK,  RGB_TOG,    RGB_MOD,        KC_HOME,     KC_PGDOWN,   KC_PGUP,   KC_END,       LOPT(LCTL(KC_SPACE)),
    _______,    KC_AUDIO_MUTE,        KC_AUDIO_VOL_DOWN,  KC_AUDIO_VOL_UP,        KC_MEDIA_PLAY_PAUSE,  _______,    LCMD(KC_LEFT),  KC_LEFT,     KC_DOWN,     KC_UP,     KC_RIGHT,     LCMD(KC_RIGHT),
    _______,    KC_MS_WH_LEFT,        KC_MS_WH_UP,        KC_MS_WH_DOWN,          KC_MS_WH_RIGHT,       _______,    KC_MS_BTN1,     KC_MS_LEFT,  KC_MS_DOWN,  KC_MS_UP,  KC_MS_RIGHT,  KC_MS_BTN2,
    SEQUENCER,  _______,              _______,            _______,                _______,              KC_DELETE,  KC_DELETE,      _______,     _______,     _______,   _______,      _______
),

/**
 * Qwerty: a QWERTY layout with BÉPO keycodes. Most CMD + letter shortcuts are more accessible in QWERTY.
 */
[_QWERTY] = LAYOUT_planck_grid(
    _______,  BP_Q,     BP_W,     BP_E,     BP_R,     BP_T,     BP_Y,     BP_U,     BP_I,     BP_O,     BP_P,     _______,
    _______,  BP_A,     BP_S,     BP_D,     BP_F,     BP_G,     BP_H,     BP_J,     BP_K,     BP_L,     _______,  _______,
    _______,  BP_Z,     BP_X,     BP_C,     BP_V,     BP_B,     BP_N,     BP_M,     _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______
),

/**
 * Sequencer: program and play MIDI sequences.
 */
[_SEQUENCER] = LAYOUT_planck_grid(
    SQ_SALL,    SQ_S(0),   SQ_S(1),   SQ_S(2),   SQ_S(3),   SQ_T(KICK),    SQ_T(SNARE),   SQ_S(4),   SQ_S(5),   SQ_S(6),   SQ_S(7),   SQ_TMPU,
    SQ_SCLR,    SQ_S(8),   SQ_S(9),   SQ_S(10),  SQ_S(11),  SQ_T(HIHAT1),  SQ_T(HIHAT2),  SQ_S(12),  SQ_S(13),  SQ_S(14),  SQ_S(15),  SQ_TMPD,
    XXXXXXX,    SQ_S(16),  SQ_S(17),  SQ_S(18),  SQ_S(19),  SQ_T(CRASH),   SQ_T(RIDE),    SQ_S(20),  SQ_S(21),  SQ_S(22),  SQ_S(23),  SQ_RESU,
    SEQUENCER,  SQ_S(24),  SQ_S(25),  SQ_S(26),  SQ_S(27),  SQ_TOG,        SQ_TOG,        SQ_S(28),  SQ_S(29),  SQ_S(30),  SQ_S(31),  SQ_RESD
)

};

/**
 * This is not a layer but simply a helper to map a (row, column) position to a LED number,
 * since the keys’ coordinates are quite counter-intuitive.
 */
const uint8_t led_map[MATRIX_ROWS][MATRIX_COLS] = LAYOUT_planck_grid(
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,
    12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
    24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,
    36,  37,  38,  39,  40,  41,  41,  42,  43,  44,  45,  46
);
// clang-format on

#define RGB_BLACK 0, 0, 0

void rgb_matrix_indicators_sequencer(void) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t column = 0; column < MATRIX_COLS; column++) {
            uint8_t  led_index = led_map[row][column];
            uint16_t keycode   = keymaps[_SEQUENCER][row][column];

            switch (keycode) {
                case SQ_TOG:
                case SEQUENCER:
                    break;
                case SQ_SALL:
                case SQ_SCLR:
                    rgb_matrix_set_color(led_index, RGB_RED);
                    break;
                case SQ_TMPD:
                case SQ_TMPU:
                    rgb_matrix_set_color(led_index, RGB_BLUE);
                    break;
                case SQ_RESD:
                case SQ_RESU:
                    rgb_matrix_set_color(led_index, RGB_CYAN);
                    break;
                case SEQUENCER_TRACK_MIN ... SEQUENCER_TRACK_MAX:
                    if (is_sequencer_track_active(keycode - SEQUENCER_TRACK_MIN)) {
                        rgb_matrix_set_color(led_index, RGB_ORANGE);
                    } else {
                        rgb_matrix_set_color(led_index, 0x20, 0x10, 0x00);
                    }
                    break;
                case SEQUENCER_STEP_MIN ... SEQUENCER_STEP_MAX:
                    if (is_sequencer_on() && sequencer_get_current_step() == (keycode - SEQUENCER_STEP_MIN)) {
                        rgb_matrix_set_color(led_index, RGB_PURPLE);
                    } else if (is_sequencer_step_on(keycode - SEQUENCER_STEP_MIN)) {
                        rgb_matrix_set_color(led_index, RGB_WHITE);
                    } else {
                        rgb_matrix_set_color(led_index, RGB_BLACK);
                    }
                    break;
                default:
                    rgb_matrix_set_color(led_index, RGB_BLACK);
                    break;
            }
        }
    }
}

void rgb_matrix_indicators_user(void) {
    if (layer_state_is(_SEQUENCER)) {
        rgb_matrix_indicators_sequencer();
    }
}

layer_state_t layer_state_set_user(layer_state_t state) { return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST); }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        /**
         * Most CMD + letter shortcuts are more accessible in QWERTY.
         */
        case KC_LCMD:
        case KC_RCMD:
            if (record->event.pressed && layer_state_is(_BASE)) {
                layer_on(_QWERTY);
            } else if (!record->event.pressed) {
                layer_off(_QWERTY);
            }
            break;
    }

    return true;
}

void keyboard_post_init_user(void) {
    // Reset the octave offset to 0
    midi_config.octave = MI_OCT_0 - MIDI_OCTAVE_MIN;

    // Configure the sequencer to use the notes defined in the beginning of this file
    sequencer_set_track_notes(planck_sequencer_track_notes);

    // Sets the initial tempo to something that is closer to what I expect
    sequencer_set_tempo(100);
}
