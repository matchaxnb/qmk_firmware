/**
# MIDI handling

## `void manage_midi_clock(void)`

If matchapad_state.MIDI_MODE is set to MASTER, emit clock messages
Else if matchapad_state.MIDI_MODE is set to LISTEN, listen to clock
messages and adjust the internal clock to follow them.
Else don't handle MIDI clock at all

## `void save_config_with_sysex(void)`

Our Manufacturer ID is defined by default to 7D (private/experimenting).

Emit the following sequence of data over sysex (F0 xxx F7) so that
Patch Librarian can consume them:

- 0x23
- matchapad state
- 0x07
- matchapad sequencer A (8)
- 0xAC
- matchapad sequencer B (16)
- 0xAB
- matchapad sequencer C (32)
- 0x19
- matchapad sequencer D (64)
- 0x05
- matchapad pad config
- 0x23

## `void load_config_from_sysex(void)`

Restore `save_config_with_sysex` style dump to internal state.

## SysEx codes reference

*Manufacturer ID: 0x7D (private)*

Enter load config mode: 0x7D304050
Enter save config mode: 0x7D504030

Sequencer state payload:
7D: manufacturer code
00|01|02: clock mode

**/
#include QMK_KEYBOARD_H
#include "matchapad_types.h"
#include "matchapad_utils.h"
#include "sysex_tools.h"

extern MidiDevice midi_device;
extern matchapad_state_t matchapad_state;
extern matchapad_internal_state_t matchapad_internal_state;


void manage_midi_clock(void) {
  switch (matchapad_state.midi_mode) {
    case MODE_MASTER:
      break;
    case MODE_LISTEN:
      break;
    case MODE_UNSET:
    default:
      break;
  }
}

void dumpstuff(char * prefix, void* pointer, int len) {
  int i = 0;
  uprintf("%s>0x", prefix);
  for (; i < len; i++) {
    uprintf("%02x", ((uint8_t *)pointer)[i]);
  }
  uprintln("<");
}

void dumpstuff2(char * prefix, void* pointer, int len) {
  int i = 0;
  dprintf("%s>0x", prefix);
  for (; i < len; i++) {
    uprintf("%02x", ((uint8_t *)pointer)[i]);
  }
  uprintln("<");
}

void save_config_with_sysex(void) {
  uint16_t size_to_send;
  uint8_t* sysex_message;
  uint8_t* config_message;
  size_t mesgsize = sizeof(uint8_t) + sizeof(matchapad_state_t);
  config_message = malloc(mesgsize);
  config_message[0] = 0x7D;
  memcpy(config_message + 1, (uint8_t*) &matchapad_state, sizeof(matchapad_state_t));
  sysex_message = malloc(sysex_encoded_length(mesgsize));
  uprintln("save_config_with_sysex");
  size_to_send = sysex_encode(sysex_message, config_message, mesgsize);
  // do the send
  midi_send_array(&midi_device, 4, (uint8_t[]){0xF0, 0x7D, 0x23, 0xF7});
  midi_send_array(&midi_device, size_to_send, sysex_message);
  dumpstuff("save_config_with_sysex:sent", sysex_message, size_to_send);
  midi_send_array(&midi_device, 4, (uint8_t[]){0xF0, 0x7D, 0x07, 0xF7});
  // TODO: send more data (sequencer data)
}

void load_config_from_sysex(void) {
  matchapad_internal_state.load_sysex = 0xFF;
}

int compare_sysex_buf(uint8_t size, ...) {
  va_list args;
  va_start(args, size);
  int outcome;
  uint8_t * toCompare = malloc((2 + size) * sizeof(uint8_t));
  toCompare[0] = 0xF0;
  for (outcome = 0; outcome < size; outcome++) {
    toCompare[outcome + 1] = va_arg(args, int);
  }
  toCompare[outcome + 1] = 0xF7;
  va_end(args);
  outcome = memcmp(toCompare, matchapad_internal_state.sysex_guard, size + 2);
  free(toCompare);
  return outcome;
}

#define _CHECK_SYSEX_GUARD(val) (compare_sysex_buf(1, (val)) == 0)
#define _CHECK_SYSEX_GUARD_2(val1, val2) (compare_sysex_buf(2, (val1), (val2)) == 0)
#define _CHECK_SYSEX_GUARD_3(val1, val2, val3) (compare_sysex_buf(3, (val1), (val2), (val3)) == 0)
#define _CHECK_SYSEX_GUARD_4(val1, val2, val3, val4) (compare_sysex_buf(4, (val1), (val2), (val3), (val4)) == 0)

void matchapad_process_sysex(MidiDevice *device, uint16_t start_byte, uint8_t data_length, uint8_t *data) {
  void * matchapad_state_pointer = &matchapad_state;
  dumpstuff2("matchapad_process_sysex called", data, data_length);
  
  // first copy the sysex message to the sysex buffer
  memcpy(matchapad_internal_state.sysex_guard + start_byte, data, data_length);
  if (matchapad_internal_state.sysex_guard[start_byte + data_length - 1] != 0xF7) {
    uprintf("s %d l %d v %02X\n", start_byte, data_length, matchapad_internal_state.sysex_guard[start_byte + data_length]);
    uprintln("sysex in progress");
    return;
  }
  switch (matchapad_internal_state.load_sysex) {
    case 0xFF: // initial
      if (_CHECK_SYSEX_GUARD_2(0x7D, 0x23)) {
        dprintln("matchapad_process_sysex: got sysex 0x23 -- starting");
        matchapad_internal_state.load_sysex = 0x01;
      }
    break;
    case 0x01: // step 1 - receive matchapad_state
      dprintf("matchapad_process_sysex: writing matchapad_state[%d:%d]\n", start_byte, data_length - 1);
      matchapad_state_dump();
      memcpy(matchapad_state_pointer, matchapad_internal_state.sysex_guard + 2, sizeof(matchapad_state_t));
      dprintln("matchapad_process_sysex: done writing matchapad_state");
      matchapad_internal_state.load_sysex = 0x02;
    break;
    case 0x02: // step 2 - 
      if (_CHECK_SYSEX_GUARD_2(0x7D, 0x07)) {
        dprintln("matchapad_process_sysex: got sysex 0x07 -- continuing");
        matchapad_internal_state.load_sysex = 0x03;
      }
    break;
    case 0x03: // step 3
      matchapad_internal_state.load_sysex = 0;
      // TODO: more steps
    break;
    case 0x00: // generic mode
      dprintln("generic sysex handler");
      if (_CHECK_SYSEX_GUARD_4(0x7D, 0x30, 0x40, 0x50)) {
        dprintln("magic sysex received, switching to load mode");
        matchapad_internal_state.load_sysex = 0xFF;
      } else if (_CHECK_SYSEX_GUARD_4(0x7D, 0x50, 0x40, 0x30)) {
        dprintln("magic sysex received, switching to dump mode");
        save_config_with_sysex();
      } else {
        dumpstuff("meh ", data, data_length);
      }
      
    break;
    default:
      uprintln("abnormal sysex state");
  }
  matchapad_state_dump();
}

void matchapad_init_midi(void) {
  matchapad_internal_state.sysex_guard = malloc(SYSEX_BUF_SIZE * sizeof(uint8_t));
  midi_register_sysex_callback(&midi_device, &matchapad_process_sysex);
}

