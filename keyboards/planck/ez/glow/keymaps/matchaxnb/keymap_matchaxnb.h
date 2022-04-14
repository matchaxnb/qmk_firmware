#ifndef _KEYMAP_MATCHAXNB_H
#define _KEYMAP_MATCHAXNB_H
void keyboard_pre_init_user(void) ;
void keyboard_post_init_user(void) ;
void display_step_matrix(void) ;
void set_step_for_track(uint8_t step, uint8_t track, bool value) ;
void toggle_step_for_track(uint8_t step, uint8_t track) ;
void toggle_step_for_active_track(uint8_t step) ;
void trigger_override_attention_track(uint8_t track) ;
void check_release_timers(void) ;
void activate_track_custom(uint8_t track) ;
void set_middle_tracks_color(void) ;
void pulse_active_tracks_cb(void) ;
void pulse_active_tracks(void) ;
void pulse_active_tracks_old(bool is_transitioning) ;
void set_track_color(uint8_t track_num, bool state) ;
void inside_step(uint8_t step) ;
void transition_step(void) ;
void apply_rgb_matrix_to_tracks(const RGB **colors) ;
void display_function_keys(void) ;
void display_paused_seq(void) ;
void apply_rgb_matrix_to_steps(const RGB **colors) ;
void display_paused_note_matrix(void) ;
void rgb_matrix_indicators_user(void) ;
void trigger_attention_timer(void) ;
void s_set_tempo(uint8_t tempo);
void s_increase_tempo(void);
void s_decrease_tempo(void);
#endif
