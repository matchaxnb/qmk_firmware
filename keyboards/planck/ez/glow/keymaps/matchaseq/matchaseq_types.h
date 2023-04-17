#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"


typedef uint8_t t_time_division;

typedef struct {
    uint8_t channels[12] ;
} Step;

typedef struct {
    uint8_t midi_values[12];
    bool muted[12]
} Channels;

typedef struct {
    uint8_t length: 5; // max value: 32
    uint8_t current_step: 5;
    uint8_t tempo;
    Step steps[MAX_PATTERN_LENGTH];
    Channels channels;
    t_time_division time_division;
} Pattern;

// states for the playback machine
typedef enum __attribute__((packed)) {
    MS_P_STOPPED, // not playing, playhead at start
    MS_P_PAUSED,  // not playing, playhead anywhere
    MS_P_PLAYING, // playing
}  matchaseq_states_playback;

typedef enum __attribute__((packed)) {
    PLAY_FORWARD,
    PLAY_REVERSE
} playback_direction;

// this holds the main sequencer state for playback
typedef struct {
    matchaseq_states_playback state;
    playback_direction play;
    uint16_t total_runtime;
} MatchaSequencerPlaybackState;

// states for the sequencer interactions
typedef enum __attribute__((packed)) {
    MS_P_DIV_MOD,   // pushing DIV      time division
    MS_P_VEL_MOD,   // pushing VEL      note velocity
    MS_P_LAST_MOD,  // pushing LAST     last note
    MS_P_BPM_MOD,   // pushing BPM      tempo
    MS_P_COPY,      // pushing COPY     copy feature
    MS_P_SEL,       // pushing SEL      select pattern
    MS_R_PAGE,      // releasing PAGE   switch page
    MS_R_MODE,      // releasing MODE   switch sequencer mode
    MS_P_MUTEA,     // pushing MUTEA    mute A section
    MS_P_MUTEB,     // pushing MUTEB    mute B section
    MS_P_BANK,      // pushing BANK     switch bank
    MS_P_AB,        // pushing one of A1...A16 or B1...B16
} matchaseq_states_sequencer_pad;



typedef struct {
    // bit field holding the list of pressed modifiers in the 12 LSB
    // 4 MSB are reserved for future use
    /* union {
        uint16_t mask;
        struct {
            bool msb1, msb2, msb3, msb4;
            bool div,
                 vel,
                 last,
                 bpm,
                 copy,
                 sel,
                 page,
                 mode,
                 mutea,
                 muteb,
                 bank,
                 notes;
        } buttons;
    } __attribute__((packed, scalar_storage_order("little-endian"))) pressed; */
    uint16_t mask;
    uint16_t maskPrev;

} MatchaSequencerPadInputState;
