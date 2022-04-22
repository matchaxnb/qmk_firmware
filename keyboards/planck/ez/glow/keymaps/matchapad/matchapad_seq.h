#pragma once
#include "matchapad_types.h"
void matchapad_seq8_init(void);
void matchapad_seq8_set_step(matchapad_step step, matchapad_seq8_program * program, matchapad_seq8_step stepmap);
void matchapad_seq8_load_test_program(void);
void matchapad_seq8_debug(void);
void matchapad_seq8_show_step(matchapad_seq8_step step);
matchapad_seq8_step matchapad_seq8_next_step(void);
void matchapad_seq8_play_next_step(void);
