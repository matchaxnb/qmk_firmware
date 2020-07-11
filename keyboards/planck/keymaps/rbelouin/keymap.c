#include QMK_KEYBOARD_H
#include "muse.h"
#include "keymap_bepo.h"

enum planck_layers { _BASE, _LOWER, _RAISE, _ADJUST };

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

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
    RESET,    KC_MEDIA_PREV_TRACK,  KC_MEDIA_REWIND,    KC_MEDIA_FAST_FORWARD,  KC_MEDIA_NEXT_TRACK,  RGB_TOG,    RGB_MOD,        KC_HOME,     KC_PGDOWN,   KC_PGUP,   KC_END,       LOPT(LCTL(KC_SPACE)),
    _______,  KC_AUDIO_MUTE,        KC_AUDIO_VOL_DOWN,  KC_AUDIO_VOL_UP,        KC_MEDIA_PLAY_PAUSE,  _______,    LCMD(KC_LEFT),  KC_LEFT,     KC_DOWN,     KC_UP,     KC_RIGHT,     LCMD(KC_RIGHT),
    _______,  KC_MS_WH_LEFT,        KC_MS_WH_UP,        KC_MS_WH_DOWN,          KC_MS_WH_RIGHT,       _______,    KC_MS_BTN1,     KC_MS_LEFT,  KC_MS_DOWN,  KC_MS_UP,  KC_MS_RIGHT,  KC_MS_BTN2,
    _______,  _______,              _______,            _______,                _______,              KC_DELETE,  KC_DELETE,      _______,     _______,     _______,   _______,      _______
)

};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) { return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST); }

bool process_record_user(uint16_t keycode, keyrecord_t *record) { return true; }
