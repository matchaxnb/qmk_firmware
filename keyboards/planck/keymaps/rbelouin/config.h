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
