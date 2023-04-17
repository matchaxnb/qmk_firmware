#pragma once
#include "constants.h"
#include "matchapad_types.h"

// management functions
void load_pattern(uint8_t slot, void *pattern_data)
void initialize_pattern(uint8_t slot);

// playback state transition functions
void play_one_step(void);
void stop_playing(void);
void reset_playhead(void);
void toggle_play(void);

// sequence management
bool toggle_step(uint8_t step);
uint8_t get_step_value(uint8_t step);
