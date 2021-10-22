#pragma once

#ifdef AUDIO_ENABLE
#    define STARTUP_SONG SONG(NO_SOUND)
#endif

#define MIDI_BASIC
#define MIDI_ADVANCED

// Most tactile encoders have detents every 4 stages
#define ENCODER_RESOLUTION 4

#define SEQUENCER_STEPS 32
#define SEQUENCER_TRACKS 6

#define ORYX_CONFIGURATOR
#define PLANCK_EZ_LED_LOWER 1
#define PLANCK_EZ_LED_RAISE 2
#define PLANCK_EZ_LED_ADJUST 3
