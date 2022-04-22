#pragma once
#include QMK_KEYBOARD_H
/**
  This defines the data structure for the matchapad sequencer.

  
**/

#define SYSEX_BUF_SIZE 32
typedef enum {
  LEN_8 = 8,
  LEN_16 = 16,
  LEN_32 = 32,
} matchapad_program_length;

typedef enum {
  MODE_MASTER,
  MODE_LISTEN,
  MODE_UNSET,
} matchapad_midi_clock_mode;

typedef enum {
  MODE_SEQ8,
  MODE_SEQ16,
  MODE_SEQ32,
  MODE_SEQ64,
  MODE_PAD,
  MODE_LAUNCHER,
  MODE_PIANOKEYS,
} matchapad_mode;

typedef enum {
  SAVE_SLOT_1,
  SAVE_SLOT_2,
  SAVE_SLOT_3,
  SAVE_SLOT_4,
} matchapad_save_slot;


typedef enum {
  PROGRAM_SEQUENCER,
  PROGRAM_LAUNCHER,
  PROGRAM_PAD,
  PROGRAM_PIANOKEYS,
} matchapad_program_type;

typedef uint8_t matchapad_step;

// it's crucial to keep these packed because pointer arithmetics.
typedef struct __attribute__((__packed__)) {
  bool i1_on : 1;
  bool i2_on : 1;
  bool i3_on : 1;
  bool i4_on : 1;
  bool i5_on : 1;
  bool i6_on : 1;
  bool i7_on : 1;
  bool i8_on : 1;
  bool i9_on : 1;
  bool i10_on : 1;
  bool i11_on : 1;
  bool i12_on : 1;
} matchapad_seq8_step;

// it's crucial to keep these packed because pointer arithmetics.
typedef struct __attribute__((__packed__)) {
  matchapad_seq8_step step1;
  matchapad_seq8_step step2;
  matchapad_seq8_step step3;
  matchapad_seq8_step step4;
  matchapad_seq8_step step5;
  matchapad_seq8_step step6;
  matchapad_seq8_step step7;
  matchapad_seq8_step step8;
} matchapad_seq8_program;

typedef uint8_t matchapad_program_slot;

typedef struct {
  matchapad_program_length length;
  matchapad_program_type type;
  
} matchapad_program;

typedef struct {
  uint8_t active_program;
} matchapad_config_t;

typedef struct {
  uint8_t load_sysex;
  uint8_t * sysex_guard;
  matchapad_step current_step;
  matchapad_seq8_program * seq8_active_program;
  matchapad_seq8_step seq8_last_step;
} matchapad_internal_state_t;


typedef struct {
  matchapad_midi_clock_mode midi_mode;
  matchapad_mode active_mode;
} matchapad_state_t;
