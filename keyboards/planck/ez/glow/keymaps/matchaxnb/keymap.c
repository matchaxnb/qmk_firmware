#include QMK_KEYBOARD_H
#include "eeprom.h"
#include "keymap_matchaxnb.h"
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
  INST6,
};

// display mods that we support
typedef enum {
  MODE_DEFAULT,
  MODE_ALL_INSTRUMENTS,
  MODE_ONE_INSTRUMENT,
} matcha_sequencer_display_mode;

// state for this keymap / instrument
typedef struct {
  uint8_t active_step;
  uint8_t active_track;
  uint8_t seen_track_of_interest;
  uint8_t last_track_of_focus;
  uint8_t tracks_with_notes; // active tracks mean tracks 
  matcha_sequencer_display_mode display_mode;
} matcha_sequencer_state_t;
// external imports

typedef struct {
  uint16_t attention_timer; // ATTENTION_MS
  uint16_t timer_play_pause; // QUIET_TIMER
  uint16_t midi_clock_interval;
  deferred_token midi_clock_handle;
  uint16_t timer_event_1; // EYEBLINK_TIMER
  uint16_t timer_event_2; // 4-beat timer (computed)
  uint8_t timer_event_1_context; // hold some context
  uint8_t timer_event_2_context; // hold some context
  uint8_t override_attention_track; // which track should have attention ephemerally
  uint16_t step_counter; // which step is running
  uint8_t tempo;
  void * timer_event_1_cb;
  void * timer_event_2_cb;
  const RGB** steps_desired_colors;
  const RGB** tracks_desired_colors;
  bool blink_phase;
  bool just_transitioned;
  bool flashed_lights;
  bool switched_focus;
  bool shifted;
} matcha_sequencer_internal_state_t;

extern rgb_config_t rgb_matrix_config;
extern rgb_config_t rgb_matrix_config;
extern sequencer_config_t sequencer_config;
extern MidiDevice midi_device;
// keycodes defines
// set a mapping of tracks to KC_KP_1...
#define MSQ_T(n) (n <=  SEQUENCER_TRACKS ? KC_KP_1 + n : KC_NO)
// set a mapping of notes from A to ... 
#define MSQ_S(n) (n <=  SEQUENCER_STEPS ? KC_A + n : KC_NO)

// technical defines
// timings
#define ATTENTION_MS 200
#define QUIET_TIMER 400
#define EYEBLINK_TIMER 200

// clocking stuff
// display
#define SEQUENCER_STEPS_PER_LAYER 32

// special values
#define NO_STEP (SEQUENCER_STEPS + 1)
#define NO_TRACK (SEQUENCER_TRACKS + 1)

#define MIN_TRACK 0
#define MAX_TRACK (SEQUENCER_TRACKS - 1)

// utility macros
#define DEV_RGB(n) (n).r, (n).g, (n).b
#define DEV_RGBP(n) (n)->r, (n)->g, (n)->b
#define SET_TRACK_NOTE_ON(track, note) rgb_matrix_set_color(stepsmap[(note)], DEV_RGB(color_map_on_rgb[(track)]))
#define TRACKMASK(track) ((1 << (track)))
#define SMMIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define SMMAX(X, Y) (((X) < (Y)) ? (X) : (Y))

#define SEQ_STEP_AS_BOOL(track, step) ((sequencer_config.steps[(step)] & TRACKMASK(track)) == (TRACKMASK(track)))
#define ATTENTION_TIMER_ACTIVE (internal_state.attention_timer > 0)
#define TIMER_1_ACTIVE (internal_state.timer_event_1 > 0 && internal_state.timer_event_1_cb != NULL)
#define TIMER_2_ACTIVE (internal_state.timer_event_2 > 0 && internal_state.timer_event_2_cb != NULL)
#define TIMER_1_ABOUT_TRACK(t) (internal_state.timer_event_1_context & ((1 << (t) ) == (1 << (t) )))
#define TIMER_2_ABOUT_TRACK(t) (internal_state.timer_event_2_context & ((1 << (t) ) == (1 << (t) )))

#define SHIFTED_OFFSET (internal_state.shifted ? SEQUENCER_STEPS_PER_LAYER : 0)

// musical defines
#define BAR_LENGTH 4
#define BAR_DURATION (sequencer_get_beat_duration() * BAR_LENGTH)

// midi defines
#define MIDI_CLOCK_PPQN 4

// aesthetic defines
// orange
#define COLOR_TRACK1_ON  33,255,255
#define COLOR_TRACK1_ON_RGB 255,198,0
#define COLOR_TRACK1_OFF 33,255,64
#define COLOR_TRACK1_OFF_RGB 64,50,0
// pink
#define COLOR_TRACK2_ON  209,255,255
#define COLOR_TRACK2_ON_RGB 234,0,255
#define COLOR_TRACK2_OFF 209,255,64
#define COLOR_TRACK2_OFF_RGB 59,0,64
// purple
#define COLOR_TRACK3_ON  185,255,255
#define COLOR_TRACK3_ON_RGB 90,0,255
#define COLOR_TRACK3_OFF 185, 255, 64
#define COLOR_TRACK3_OFF_RGB 23,0,64
// red
#define COLOR_TRACK4_ON  0,255,255
#define COLOR_TRACK4_ON_RGB 255,0,0
#define COLOR_TRACK4_OFF 0,255, 64
#define COLOR_TRACK4_OFF_RGB 64,0,0
// green
#define COLOR_TRACK5_ON  85,255,255
#define COLOR_TRACK5_ON_RGB 0,255,0
#define COLOR_TRACK5_OFF 85,255,64
#define COLOR_TRACK5_OFF_RGB 0,64,0
// blue
#define COLOR_TRACK6_ON  170,255,255
#define COLOR_TRACK6_ON_RGB 0,0,255
#define COLOR_TRACK6_OFF 170,255,64
#define COLOR_TRACK6_OFF_RGB 0,0,64


// current note (RGB)
#define COLOR_CURRENTNOTE 0,0,100
#define COLOR_CURRENTNOTE_RGB 255,255,255

// #define COLOR_CURRENTNOTE_RGB 124, 13, 14
const RGB color_curnote = { COLOR_CURRENTNOTE_RGB };
const RGB color_curnote_shift = { RGB_CHARTREUSE };

// note set elsewhere (pink)
#define COLOR_SETNOTE 219, 252, 252
#define COLOR_SETNOTE_RGB 252,3,214

#define PRINTLN uprintln
#define PRINTF uprintf
// constants for colors
const RGB color_setnote = { COLOR_SETNOTE_RGB };
const RGB color_offnote = { RGB_BLACK };

// color map for notes that are set and active
const HSV PROGMEM color_map_on[SEQUENCER_TRACKS] = {
 { COLOR_TRACK1_ON },
 { COLOR_TRACK2_ON },
 { COLOR_TRACK3_ON },
 { COLOR_TRACK4_ON },
 { COLOR_TRACK5_ON },
 { COLOR_TRACK6_ON },
};

// RGB (computed with a preprocessor)
const RGB PROGMEM color_map_on_rgb[SEQUENCER_TRACKS] = {
 { COLOR_TRACK1_ON_RGB },
 { COLOR_TRACK2_ON_RGB },
 { COLOR_TRACK3_ON_RGB },
 { COLOR_TRACK4_ON_RGB },
 { COLOR_TRACK5_ON_RGB },
 { COLOR_TRACK6_ON_RGB },
};

// color map for notes that are set but not currently active
const HSV PROGMEM color_map_off[SEQUENCER_TRACKS] = {
 { COLOR_TRACK1_OFF },
 { COLOR_TRACK2_OFF },
 { COLOR_TRACK3_OFF },
 { COLOR_TRACK4_OFF },
 { COLOR_TRACK5_OFF },
 { COLOR_TRACK6_OFF },
};

// RGB (computed with a preprocessor)
const RGB PROGMEM color_map_off_rgb[SEQUENCER_TRACKS] = {
 { COLOR_TRACK1_OFF_RGB },
 { COLOR_TRACK2_OFF_RGB },
 { COLOR_TRACK3_OFF_RGB },
 { COLOR_TRACK4_OFF_RGB },
 { COLOR_TRACK5_OFF_RGB },
 { COLOR_TRACK6_OFF_RGB },
};

// MIDI notes for instruments - setting them 1 octave apart
// so if you use MIDI to CV you can get 1V offsets
const uint16_t seq_instruments_notes[SEQUENCER_TRACKS] = {
  [INST1] = MI_C, [INST2] = MI_C_1,
  [INST3] = MI_C_2, [INST4] = MI_C_3,
  [INST5] = MI_C_4, [INST6] = MI_C_5,
};

/* planck grid
  0  1  2  3  4  5  6  7  8  9  10 11
  12 13 14 15 16 17 18 19 20 21 22 23
  24 25 26 27 28 29 30 31 32 33 34 35
  36 37 38 39 40 41 41 42 43 44 45 46
*/

#define PLAY_PAUSE_LED 41
#define SHIFT_TRACK_LED 0
#define CLEAR_TRACK_LED 24
#define RESET_LED 36

#define COLOR_PLAY_PAUSE_1 RGB_PINK
#define COLOR_PLAY_PAUSE_2 RGB_CYAN
#define COLOR_SHIFT_TRACK_1 RGB_ORANGE
#define COLOR_SHIFT_TRACK_2 RGB_TURQUOISE
#define COLOR_CLEAR_TRACK RGB_GOLD
#define COLOR_RESET RGB_CORAL



const uint8_t PROGMEM stepsmap[DRIVER_LED_TOTAL] = {
   1, 13, 25, 37, // bar 1
   2, 14, 26, 38, // bar 2
   3, 15, 27, 39, // bar 3
   4, 16, 28, 40, // bar 4
   7, 19, 31, 42, // bar 5
   8, 20, 32, 43, // bar 6
   9, 21, 33, 44, // bar 7
  10, 22, 34, 45  // bar 8
};

const uint8_t PROGMEM tracksmap[SEQUENCER_TRACKS] = {
  5, 17, 29, // tracks 1, 2, 3
  6, 18, 30, // tracks 4, 5, 6
};


// add a RESET key on the trident so I can dev
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_planck_grid(
    KC_S, SQ_S(0), SQ_S(4), SQ_S(8), SQ_S(12), SQ_T(INST1), SQ_T(INST4), SQ_S(16), SQ_S(20), SQ_S(24), SQ_S(28), SQ_TMPD,
    SQ_SCLR, SQ_S(1), SQ_S(5), SQ_S(9), SQ_S(13), SQ_T(INST2), SQ_T(INST5), SQ_S(17), SQ_S(21), SQ_S(25), SQ_S(29), SQ_TMPU,
    DEBUG,   SQ_S(2), SQ_S(6), SQ_S(10), SQ_S(14), SQ_T(INST3), SQ_T(INST6), SQ_S(18), SQ_S(22), SQ_S(26), SQ_S(30), SQ_RESD,
    RESET,   SQ_S(3), SQ_S(7), SQ_S(11), SQ_S(15), SQ_TOG,            KC_NO, SQ_S(19), SQ_S(23), SQ_S(27), SQ_S(31), SQ_RESU
  ),
};


/* putting that into a struct */


matcha_sequencer_state_t matcha_sequencer_state = {
  .active_step =  NO_STEP,  // active_step
  .active_track = NO_TRACK, 
  .seen_track_of_interest = 0,           // seen_track_of_interest
  .last_track_of_focus = 0,                    // last track of focus
  .tracks_with_notes = 0,                    // active tracks mask
  .display_mode = MODE_DEFAULT,
};

matcha_sequencer_internal_state_t internal_state = {
  .attention_timer = 0,        // attention_timer; // ATTENTION_MS
  .timer_play_pause = 0,        // timer_play_pause; // QUIET_TIMER
  .midi_clock_interval = 1000 / 96,
  .midi_clock_handle = 0,
  .timer_event_1 = 0,        // timer_event_1; // EYEBLINK_TIMER
  .timer_event_2 = 0,        // timer_event_2; // 4-beat (unimplemented)
  .timer_event_1_context =0,        // timer_event_1_context; // bitmask for some context passing
  .timer_event_2_context = 0,        // timer_event_2_context; // 
  .override_attention_track = NO_TRACK, // override_attention_track; 
  .step_counter = NO_STEP,  // step_counter; // count steps and loop sometime in the highs
  .tempo = 120,
  .timer_event_1_cb = NULL,     // callback 1 timer_event_1_cb;
  .timer_event_2_cb = NULL,     // callback 2 timer_event_2_cb;
  .steps_desired_colors = NULL,     // RGB** steps_desired_colors;
  .tracks_desired_colors = NULL,     // RGB** tracks_desired_colors;
  .blink_phase = false,    // blink phase
  .just_transitioned = true,     // just_transitioned;                
  .flashed_lights = false,    // flashed lights    flashed_lights;                   
  .switched_focus = true,
  .shifted = false,
};                                                  
                                                    
void initialize_state_and_config(void) {
  internal_state.steps_desired_colors = malloc(sizeof(RGB) * SEQUENCER_STEPS_PER_LAYER);
  internal_state.tracks_desired_colors = malloc(sizeof(RGB) * SEQUENCER_TRACKS);
  // default res: 16 / beat
}

void keyboard_pre_init_user(void) {
  initialize_state_and_config();
}

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
  sequencer_set_track_notes(seq_instruments_notes);
  trigger_attention_timer();
  matcha_sequencer_state.active_step = SEQUENCER_STEPS + 1;
  sequencer_config.resolution = SQ_RES_16;
  s_set_tempo(100);
  internal_state.midi_clock_handle = defer_exec(internal_state.midi_clock_interval, &handle_midi_clock, NULL);
}

void refresh_midi_interval(void) {
  internal_state.midi_clock_interval = 60000 / internal_state.tempo / 4 / MIDI_CLOCK_PPQN;
}

void s_set_tempo(uint8_t tempo) {
  internal_state.tempo = tempo;
  sequencer_set_tempo(tempo);
  refresh_midi_interval();
  PRINTF("s_set_tempo: %d\n", tempo);
}

void s_decrease_tempo(void) {
  sequencer_decrease_tempo();
  internal_state.tempo = sequencer_get_tempo();
  refresh_midi_interval();
  PRINTF("s_decrease_tempo: %d\n", internal_state.tempo);
  
}

void s_increase_tempo(void) {
  sequencer_increase_tempo();
  internal_state.tempo = sequencer_get_tempo();
  refresh_midi_interval();
  PRINTF("s_increase_tempo: %d\n", internal_state.tempo);
}

void toggle_shift_seq(void) {
  internal_state.shifted = !internal_state.shifted;
  PRINTF("toggle_shift_seq: %b\n", internal_state.shifted);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  uint8_t i = 0;
  if (!record->event.pressed) {
    return true;
  }
  bool track_is_on;
  switch (keycode) {
    case KC_S:
      toggle_shift_seq();
      return false;
      break;
    case SQ_TMPU:
      s_increase_tempo();
      return false;
      break;
    case SQ_TMPD:
      s_decrease_tempo();
      return false;
      break;
    case MSQ_T(INST1) ... MSQ_T(INST6):
      sequencer_toggle_track_activation(keycode - MSQ_T(INST1));
      track_is_on = is_sequencer_track_active(keycode - MSQ_T(INST1));
      PRINTF ("toggle track %d: %b\n", keycode - MSQ_T(INST1), track_is_on);
      return false;
      break;
    case SQ_T(INST1) ... SQ_T(INST6):
      activate_track_custom(keycode - SQ_T(INST1));
      return false;
      break;
    case SQ_S(0) ... SQ_S(31):
      if (internal_state.shifted) {
        keycode += 32;
      }
      toggle_step_for_active_track((keycode - SQ_S(0)));
      return false;
      break;
    case DEBUG:
      PRINTLN ("resetting the sequencer steps");
      for (i = 0; i < SEQUENCER_STEPS; i++) {
        sequencer_config.steps[i] = 0;
      }
      PRINTLN ("done");

  }
  return true;
}

void display_step_matrix(void) {
  int i, j;
  for (i = 0; i < SEQUENCER_TRACKS; i++) {
    PRINTF ("==== Track %d ====\n", i);
    for (j = 0; j < 3; j++) {
      PRINTF ("[%b] [%b] [%b] [%b] [ ] [ ] [%b] [%b] [%b] [%b]\n",
              SEQ_STEP_AS_BOOL(i, j),
              SEQ_STEP_AS_BOOL(i, j + 4),
              SEQ_STEP_AS_BOOL(i, j + 8),
              SEQ_STEP_AS_BOOL(i, j + 12),
              SEQ_STEP_AS_BOOL(i, j + 16),
              SEQ_STEP_AS_BOOL(i, j + 20),
              SEQ_STEP_AS_BOOL(i, j + 24),
              SEQ_STEP_AS_BOOL(i, j + 28)
              );
    }
    PRINTF ("[%b] [%b] [%b] [%b] [ ] [ ] [%b] [%b] [%b] [%b]\n",
            SEQ_STEP_AS_BOOL(i, 3),
            SEQ_STEP_AS_BOOL(i, 7),
            SEQ_STEP_AS_BOOL(i, 11),
            SEQ_STEP_AS_BOOL(i, 15),
            SEQ_STEP_AS_BOOL(i, 19),
            SEQ_STEP_AS_BOOL(i, 23),
            SEQ_STEP_AS_BOOL(i, 27),
            SEQ_STEP_AS_BOOL(i, 31)
            );
  }
}

void set_step_for_track(uint8_t step, uint8_t track, bool value) {
  if (step < SEQUENCER_STEPS && track < SEQUENCER_TRACKS) {
    if (value) {
      sequencer_config.steps[step] |= TRACKMASK(track);
    } else {
      sequencer_config.steps[step] &= ~TRACKMASK(track);
    }
    display_step_matrix();
    PRINTF ("track %d note %d value new %b (%d)\n", track, step, value, sequencer_config.steps[step]);
  } 
  else {
    PRINTF ("invalid set_step_for_track(%d, %d, %b)", step, track, value);
  }
}


void toggle_step_for_track(uint8_t step, uint8_t track) {
  bool step_state = is_sequencer_step_on_for_track(step, track);
  set_step_for_track(step, track, !step_state);
}

void toggle_step_for_active_track(uint8_t step) {
  if (matcha_sequencer_state.active_track == NO_TRACK) {
    PRINTLN ("toggle_step_for_active_track::no active track, doing nothing.");
    return;
  }
  bool step_state = is_sequencer_step_on_for_track(step, matcha_sequencer_state.active_track);
  PRINTF ("step prev state %d: %b\n", step, step_state);
  set_step_for_track(step, matcha_sequencer_state.active_track, !step_state);
}

void trigger_override_attention_track(uint8_t track) {
  internal_state.override_attention_track = track;
}

void trigger_attention_timer(void) {
  internal_state.attention_timer = timer_read();
}

uint32_t handle_midi_clock(uint32_t trigger_time, void *cb_arg) {
  midi_send_clock(&midi_device);
  dprintf("handle_midi_clock::trigger %d\n", trigger_time);
  return internal_state.midi_clock_interval;
}

void check_release_timers(void) {
    /* release timers once they have expired their shelf life */
    /*if (internal_state.midi_clock_interval > 0 && timer_elapsed(internal_state.midi_clock_interval) > 60000 / internal_state.tempo / (MIDI_CLOCK_PPQN * 4)) {
      midi_send_clock(&midi_device);
      internal_state.midi_clock_interval = timer_read();
    }*/

    if (internal_state.attention_timer > 0 && timer_elapsed(internal_state.attention_timer) > BAR_DURATION) {
      trigger_attention_timer();
      internal_state.override_attention_track = NO_TRACK;
      dprintln("check_release_timers::attention_timer: done");
    } else {
      if (internal_state.attention_timer > 0) {
        dprintln("check_release_timers::attention_timer: still going");
      }
    }

    if (internal_state.timer_event_1 > 0 && timer_elapsed(internal_state.timer_event_1) > EYEBLINK_TIMER) {
      ((void (*)(void))internal_state.timer_event_1_cb)();
      internal_state.timer_event_1_cb = NULL;
      internal_state.timer_event_1 = 0;
      internal_state.timer_event_1_context = 0;
      dprintln ("check_release_timers::timer_event_1: done");
    }

}

void activate_track_custom(uint8_t track) {
  bool track_is_on = is_sequencer_track_active(track);
  if (!track_is_on) {
    trigger_override_attention_track(track);
  } else {
    internal_state.override_attention_track = NO_TRACK;
  }
  // do the actual activate/deactivate
  // sequencer_set_track_activation(track, !track_is_on);
  sequencer_toggle_single_active_track(track);
  matcha_sequencer_state.active_track = track;
}

void pulse_active_tracks_cb(void) {
  display_function_keys();
}

void pulse_active_tracks(void) {
  if (internal_state.timer_event_1 > 0) {
    dprintln("pulse_active_tracks::already using timer_event_1");
    return;
  }
  dprintln("pulse_active_tracks::run");
  const RGB ** colorsCopy;
  colorsCopy = internal_state.tracks_desired_colors;
  internal_state.timer_event_1_context = 0;
  for (int t = 0; t < SEQUENCER_TRACKS; t++) {
    if (is_sequencer_step_on_for_track(matcha_sequencer_state.active_step, t)) {
      // rgb_matrix_set_color(tracksmap[t], DEV_RGB(color_offnote));
      *colorsCopy = &color_offnote;
      internal_state.timer_event_1_context |= TRACKMASK(t);
    }
    colorsCopy++;
  }
  if (internal_state.shifted && matcha_sequencer_state.active_step < SEQUENCER_STEPS_PER_LAYER) {
    rgb_matrix_set_color(SHIFT_TRACK_LED, DEV_RGB(color_offnote));
    
  } else if (!internal_state.shifted && matcha_sequencer_state.active_step >= SEQUENCER_STEPS_PER_LAYER) {
    rgb_matrix_set_color(SHIFT_TRACK_LED, DEV_RGB(color_offnote));
  }
  internal_state.timer_event_1 = timer_read();
  internal_state.timer_event_1_cb = &pulse_active_tracks_cb;

}

// hsv colors, convert please
RGB get_track_color(uint8_t track_num, bool state) {
  RGB col;
  if (state) {
    col = color_map_on_rgb[track_num];
  } else {
    col = color_map_off_rgb[track_num];
  }
  return col;
}

// set track color for middle buttons, fade them if they are off
void set_track_color(uint8_t track_num, bool state) {
  if (track_num >= SEQUENCER_TRACKS) {
    PRINTLN("set_track_color::track_num invalid");
    return;
  }
  // rgb_matrix_set_color(tracksmap[track_num], DEV_RGB(color));
  const RGB ** colorCopy = internal_state.tracks_desired_colors;
  int i = 0;
  while (i++ < track_num) {
    colorCopy++;
  }
  if (state) {
    *colorCopy = &color_map_on_rgb[track_num];
  } else {
    *colorCopy = &color_map_off_rgb[track_num];
  }
}

uint8_t count_active_tracks_for_step(uint8_t step) {
  uint8_t c = 0, i;
  for (i = 0; i < SEQUENCER_TRACKS; i++) {
    if (sequencer_config.steps[step] & TRACKMASK(i)) {
      c++;
    }
  }
  return c;
}

uint8_t get_tracks_with_notes(void) {
  uint8_t active_mask = 0;
  for (uint8_t i = 0; i < SEQUENCER_STEPS; i++) {
    for (uint8_t j = 0; j < SEQUENCER_TRACKS; j++) {
      active_mask |= sequencer_config.steps[i] & TRACKMASK(j);
    }
  }
  return active_mask;
}

uint16_t get_track_ms_attention_span(void) {
  // compute how long maximum we should give attention to a track. minimum
  // will be EYEBLINK_TIMER, maximum will be 1/6th of a bar
  uint16_t max_foc = sequencer_get_beat_duration() * BAR_LENGTH / SEQUENCER_TRACKS;
  
  return SMMIN(EYEBLINK_TIMER, SMMAX(EYEBLINK_TIMER, max_foc));
}

uint8_t get_track_of_focus(void) {
  uint8_t current, newfocus;
  uint8_t active = get_tracks_with_notes();
  uint8_t i;
  bool found = false;
  if (internal_state.override_attention_track != NO_TRACK) {
    return internal_state.override_attention_track;
  }
  current = matcha_sequencer_state.last_track_of_focus;
  internal_state.switched_focus = true;
  if (timer_elapsed(internal_state.attention_timer) < sequencer_get_beat_duration()) {
    dprintln("get_track_of_focus::timer still present");
    return current;
  } else {
    if (current == MAX_TRACK) {
      i = 0;
    } else {
      i = current + 1;
    }
    for (; i <= MAX_TRACK; i++) {
      if (((1 << i) & active) == (1 << i)) {
        newfocus = i;
        found = true;
        dprintln("get_track_of_focus::found at first shot");
        break;
      }
    }
    if (!found) {
      for (i = 0; i <= MAX_TRACK; i++) {
        if (i == current) {
          continue;
        }
        if (((1 << i) & active) == (1 << i)) {
          newfocus = i;
          dprintln("get_track_of_focus::found at second shot");
          found = true;
          break;
        }
      }  
    }
    if (!found) {
      newfocus = current;
      internal_state.switched_focus = false;
    }
    matcha_sequencer_state.last_track_of_focus = newfocus;
    trigger_attention_timer();
    return newfocus;

  }
}

void inside_step(uint8_t step) {
  /**
    vocabulary:
      cur is our CURRENT step: where we are in the bars
      going means a note is active in the current track
      set means a note is active in any track. going implies set.
      off means a note is off in all tracks
  **/
  bool track_is_on;
  uint8_t track_of_interest = get_track_of_focus();
  uint8_t i, num_active_tracks, tracks_with_notes, note;
  uint8_t active_tracks_for_note;
  RGB color_going;
  color_going = color_map_off_rgb[track_of_interest];
  const RGB * note_color;
  // set state of the central track indicators
  //if (internal_state.flashed_lights) {
  //pulse_active_tracks_old(false);
  //}
  for (i = 0; i < SEQUENCER_TRACKS; i++) {
    track_is_on = is_sequencer_track_active(i);
    if (track_is_on) {
      tracks_with_notes += 1 << i;
      num_active_tracks++;
    }
    //if (!internal_state.flashed_lights) {
    //  set_track_color(i, track_is_on);
    //}
  }
  // did we change our track of interest or are in a new step?
  // if not, well, do nothing
  if (!(internal_state.switched_focus || internal_state.just_transitioned)) {
    return;
  }

  // if we have changed track of interest, we should refresh the matrix.
  // PRINTF ("switched track of interest: %d -> %d\n", matcha_sequencer_state.seen_track_of_interest, track_of_interest);
  switch (matcha_sequencer_state.display_mode) {
    case MODE_DEFAULT:
    case MODE_ALL_INSTRUMENTS:
      for (i = 0; i < SEQUENCER_STEPS_PER_LAYER; i++) {
        note = i + SHIFTED_OFFSET;
        active_tracks_for_note = count_active_tracks_for_step(note);
        
        // if we're in the current step let's have a special treatment
        if (note == step && sequencer_config.enabled) {
          if (active_tracks_for_note == 1 && is_sequencer_step_on_for_track(note, track_of_interest)) {
            note_color = &color_map_on_rgb[track_of_interest];
          }
          else {
            note_color = &color_curnote;
          }
        }
        // note is going on current track
        else if (is_sequencer_step_on_for_track(note, track_of_interest)) {
          note_color = &color_going;
        }
        // note is set
        else if (active_tracks_for_note == 1) {
          // set for exactly one track - leave it of the color of its sole track
          continue;
        }
        else if (active_tracks_for_note > 1) {
          note_color = &color_setnote;
        }
        else if (
          (internal_state.shifted && i == step - SHIFTED_OFFSET) ||
          (!internal_state.shifted && i == step + SHIFTED_OFFSET)
        ) {
            note_color = &color_curnote_shift;
        }
        // note is off
        else {
          note_color = &color_offnote;
        }
        rgb_matrix_set_color(stepsmap[i], DEV_RGBP(note_color));
      }
    break;
    case MODE_ONE_INSTRUMENT:
      
    break;
  }
  // PRINTLN ("refreshed");
  internal_state.just_transitioned = false;
}


void transition_step(void) {
      // we have changed step, run the step change actions
      //PRINTF ("num active tracks: %d\n", num_active_tracks);
      //PRINTF ("active tracks: %d\n", active_tracks);
      internal_state.step_counter += 1;
      // highest uint16_t that I can divide by 4 (default bar length)
      if (internal_state.step_counter > 65532) {
        internal_state.step_counter = 0;
      }
      //pulse_active_tracks_old(true);
      internal_state.just_transitioned = true;
      // reset attention track
      return;
}

void apply_rgb_matrix_to_tracks(const RGB **colors) {
  int i;
  for (i = 0; i < SEQUENCER_TRACKS; i++) {
    rgb_matrix_set_color(tracksmap[i], DEV_RGBP(*colors));
    colors++;
  }
}

void display_function_keys(void) {
  // display the middle block with the track pickers
  bool track_is_on;
  for (int i = 0; i < SEQUENCER_TRACKS; i++) {
    track_is_on = is_sequencer_track_active(i);
    if (track_is_on) {
      matcha_sequencer_state.tracks_with_notes |= (1 << i);      
    } else {
      matcha_sequencer_state.tracks_with_notes &= ~(1 << i);
    }
    if (TIMER_1_ABOUT_TRACK(i)) {
      continue;
    }
    set_track_color(i, track_is_on);
  }
  if (internal_state.shifted && !internal_state.just_transitioned) {
    rgb_matrix_set_color(SHIFT_TRACK_LED, COLOR_SHIFT_TRACK_2);
  } else if (!internal_state.shifted && !internal_state.just_transitioned){
    rgb_matrix_set_color(SHIFT_TRACK_LED, COLOR_SHIFT_TRACK_1);
  }
  apply_rgb_matrix_to_tracks(internal_state.tracks_desired_colors);
}

void display_paused_seq(void) {
  if (timer_elapsed(internal_state.timer_play_pause) < QUIET_TIMER) {
    return;
  }
  if (internal_state.blink_phase) {
    rgb_matrix_set_color(PLAY_PAUSE_LED, COLOR_PLAY_PAUSE_1);
    internal_state.timer_play_pause = timer_read();
  } else {
    rgb_matrix_set_color(PLAY_PAUSE_LED, COLOR_PLAY_PAUSE_2);
    internal_state.timer_play_pause = timer_read();
  }
  internal_state.blink_phase = !internal_state.blink_phase;
}


void apply_rgb_matrix_to_steps(const RGB **colors) {
  int i;
  for (i = 0; i < SEQUENCER_STEPS_PER_LAYER; i++) {
    rgb_matrix_set_color(stepsmap[i], DEV_RGBP(*colors));
    dprintln("apply_rgb_matrix_to_steps::set color");
    dprintf("rgb: %d %d %d\n", (*colors)->r, (*colors)->g, (*colors)->b);
    colors++;
  }
  dprintln("apply_rgb_matrix_to_steps::done");
}

void display_paused_note_matrix(void) {
  /* display notes when in paused state

    blank the noteboard

    for the currently active track, display all notes (skip if no active track)

    then display notes for the current "track of interest" (time-based rota)
  */
  int i;
  int focus = get_track_of_focus();
  const RGB **colorsCopy = internal_state.steps_desired_colors;
  const RGB *colorHolder;
  // blank
  for (i = 0; i < SEQUENCER_STEPS_PER_LAYER; i++) {
    *colorsCopy = &color_offnote;
    colorsCopy++;
  }
  
  // handle rotate 
  if (matcha_sequencer_state.active_track != NO_TRACK) {
    dprintln ("display_paused_note_matrix::handle active track");
    colorsCopy = internal_state.steps_desired_colors;
    colorHolder = &color_map_off_rgb[matcha_sequencer_state.active_track];
    for (i = 0 + SHIFTED_OFFSET; i < SEQUENCER_STEPS_PER_LAYER + SHIFTED_OFFSET; i++) {
      if (is_sequencer_step_on_for_track(i, matcha_sequencer_state.active_track)) {
        dprintf("display_paused_note_matrix::active::step is on %d[%d]\n", matcha_sequencer_state.active_track, i);
        *colorsCopy = colorHolder;
      }
      colorsCopy++;
    }
  }

  colorsCopy = internal_state.steps_desired_colors;
  colorHolder = &color_map_off_rgb[focus];
  for (i = 0 + SHIFTED_OFFSET; i < SEQUENCER_STEPS_PER_LAYER + SHIFTED_OFFSET; i++) {
    if (is_sequencer_step_on_for_track(i, focus)) {
      dprintf("display_paused_note_matrix::focus::step is on %d[%d]\n", focus, i);
      *colorsCopy = colorHolder;
    }
    colorsCopy++;
  }

  apply_rgb_matrix_to_steps(internal_state.steps_desired_colors);
}

void rgb_matrix_indicators_user(void) {
    /* our main loop runs here because this is a light game
    
    flow:

    detect if we just changed step.
    if we did not, run inside_step()
    if we did, run transition_step()
    */
    bool just_changed_step;
    uint8_t cur_step = sequencer_get_current_step();
    
    // first, refresh track pad.
    display_function_keys();
    // detect if we just changed step
    just_changed_step = matcha_sequencer_state.active_step != cur_step;
    if (just_changed_step) {
        matcha_sequencer_state.active_step = cur_step;
    }
    if (is_sequencer_on()) {
      if (just_changed_step) {
        pulse_active_tracks();
        transition_step();
      } else {
        inside_step(cur_step);
      }
    }
    else {
      display_paused_seq();
      display_paused_note_matrix();
    }
    // setup basic stuff
    check_release_timers();
    rgb_matrix_set_color(RESET_LED, COLOR_RESET);
    rgb_matrix_set_color(CLEAR_TRACK_LED, COLOR_CLEAR_TRACK);

}
