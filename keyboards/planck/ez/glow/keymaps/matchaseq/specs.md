# Matcha's MIDI sequencer

A 12-channel, 2 pages, 16 steps per page, 32 patterns drum sequencer for a 40% keyboard.

# How to use

## Layout

The general layout is as follows

```text
   1     2     3    4     5     6     7    8     9                  0      NUMBERS
 <--     ZONE A      --><--    controls      --><--     ZONE B      -->
 _______________________________________________________________________
| A1  | A2  | A3  | A4  |MUTEA|BANK | SEL |MUTEB| B1  | B2  | B3  | B4  |  HUNDREDS
|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
| A5  | A6  | A7  | A8  | DIV |VEL  | MODE| PAGE| B5  | B6  | B7  | B8  |  TENS
|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
| A9  | A10 | A11 | A12 |     |     |     | COPY| B9  | B10 | B11 | B12 |  UNITS
|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
| A13 | A14 | A15 | A16 | LAST| PLAYSTOP  |  BPM| B13 | B14 | B15 | B16 |
|_____|_____|_____|_____|_____|___________|_____|_____|_____|_____|_____|
```

## Modes

The sequencer has 2 modes:

- Pattern mode
- Song mode

Pattern mode allows to program a pattern of maximum 32 steps on the 12 channels.
Song mode allows to chain patterns in order to perform them one after the other.

## Switching modes

To enter one mode or the other, press the MODE key. It will cycle between 3 states:

- backlight off: this is the PERFORM mode, that only does PLAYSTOP of the programmed song or the last pattern if no song is set.
- blue backlight: SONG mode (see section Song mode operation in the manual)
- red backlight: PATTERN mode (see section Pattern mode operation in the manual)

## Pattern mode operation

To enter Pattern mode, refer to the Switching modes section. When you enter Pattern mode for the first time, you are on Pattern number 1.

### Editing a pattern

To edit a pattern, the general workflow is as follows:

- Select the pattern by chording the SEL key and one of the pattern input buttons in Zones A and B
- Set the page and total length of the pattern using the LAST button and one of the Zone steps.
- Set the wanted time division using the DIV key. The key flashes dimly on each beat and brightly at the first beat of each bar.
- Set the default tempo of the pattern using the BPM key. The key flash dimly for each quarter note.
- Select the channel bank you want to modify. Each channel bank holds 2 channels, each channel corresponding to a MIDI note value.
- Input the note patterns

Patterns are modified in real time and the changes, as such, will be reflected immediately.

### Setting page and total pattern length

Page length and total pattern length are related concepts. A pattern is made of 1 or 2 pages, and each page has the same number of steps.

In order to set the number of pages and the pattern length, proceed as follow:

- Press and hold the LAST button. The current selected setting flashes in ORANGE
- For single-page patterns, press one of the 16 Zone A step buttons.
- For dual-page patterns, press one of the 16 Zone B step buttons.
- Release the LAST button. The pattern length is applied.

### Setting time divisions

There are two families of time divisions: regular and triplets. In regular time divisions, the pattern length will default to 16 steps. In triplets, it will default to 12 steps.

You can select your time division from a broad selection.

Regular time divisions:

- A1: 1/32 (demisemiquavers)
- A2: 1/16 (semiquavers) [initial value]
- A3: 1/8 (quavers)
- A4: 1/4 (crotchets)

Triplet time divisions:

- B1: 1/16 triplets (3 of these = 2 1/16 notes = 1 1/8 note)
- B2: 1/8 triplets (3 of these = 2 1/8 notes = 1 1/4 note)
- B3: 1/4 triplets (3 of these = 2 1/4 notes)
- B4: 1/2 triplets (3 of these = 1 note = 1 4/4 bar)

For triplet time divisions, if the pattern total length isn't a multiple of 3, it will be set by default to either 12 or 24 depending on the initial setting. This can be overridden by setting it to another value.

In order to select the time division you want follow these instructions:

- Press and hold the DIV button. The current setting is highlighted and flashes in BLUE.
- Press the button that corresponds to your wanted time division. Hint: regular time divisions are in Zone A and triplets are in Zone B. Higher values mean slower notes. The newly selected value is lit in solid BLUE.
- Release the DIV button. The newly selected value is lit in solid BLUE for 1 second.

### Bank selection

Rotate through the 6 banks using the BANK button when in Pattern mode

- BANK 1 (red): MIDI 36 and 38 (corresponding to GM Bass Drum 1 and Snare Drum 1)
- BANK 2 (green): MIDI 43 and 47 (corresponding to GM Low Tom 1 and Mid Tom 1)
- BANK 3 (blue): MIDI 50 and 39 (corresponding to GM High Tom 1 and Hand Clap)
- BANK 4 (yellow): MIDI 56 and 49 (corresponding to GM Cowbell and Crash Cymbal 1)
- BANK 5 (purple): MIDI 46 and 42 (corresponding to GM Open Hi-Hat and Closed Hi-Hat)
- BANK 6 (orange): MIDI 35 and CC1 (corresponding to GM Kick Drum and MIDI modulation wheel)

### Page selection

The sequencer offers 2 pages, Page 1 and 2. Page 1 (constant white backlight) holds steps 1-16 and Page 2 (slow flashing white backlight) holds steps 17-32.

By keeping the PAGE button down and input steps, you are doing a temporary switch to the other page. When you release the PAGE button, you are back on your latched page.

Pressing the PAGE button and releasing it toggles between Page 1 and 2.

### Inputting velocity for steps

Each step has a MIDI velocity info associated with it. The default velocity is 127 but it's possible to set it to values between 3 and 127 (with increments of 4) for each step of each channel separately.

In order to select a velocity, follow these instructions:

- Press and hold the VEL button. The current velocity setting is highlighted and flashes in RED.
- Press one of the A1...A16 or B1...B16 buttons. A1: v=3, A16: v=63, B1: v=67, B16: v=127. The new value is lit in solid RED.
- Release the VEL button. All notes input from that moment will be set with the new velocity

### Globally skewing velocity for a channel

Sometimes, you want to alter velocity for a channel overall. It would be a pain to input each note again. You can apply a ratio to channel velocity for each channel without touching the note velocity value. This (positive) ratio will be applied to each note velocity when the MIDI notes are emitted. Values will of course be clamped at 127. In this mode,  velocity is immediately transformed when you change the parameter.

In order to apply a velocity ratio to the Zone A channel, do as follows:
- Press and hold the VEL button.
- To increase the velocity ratio (boost velocity), press the DIV button. To decrease it, press the LAST button.
- To reset the velocity ratio to 1, press the MUTEA button.
- Release the VEL button.

In order to apply a velocity ratio to the Zone B channel, do as follows:
- Press and hold the VEL button.
- To increase the velocity ratio, press the PAGE button. To decrease it, press the BPM button.
- To reset the velocity ratio to 1, press the MUTEB button.
- Release the VEL button.

### Inputting instrument modulation

Matcha's MIDI Sequencer can emit MIDI CC1 (MIDI Mod wheel) messages. These are applied overall to all the notes that the instrument emits. The sequencer track for these is on Bank 6, Zone B.

Modulation values are set from 0 to 127 with the following principles:
- A step with final sequencer velocity (velocity * ratio) <= 3  will emit CC1 = 0
- A step with final sequencer velocity (velocity * ratio) = 127 will emit CC1 = 127
- Intermediate values will emit proportional modulation values

### Viewing and setting pattern BPM

Matcha's MIDI Sequencer supports all integer BPM values from 40 BPM to 240 BPM.

The BPM value is displayed by lighting 1 key per row when the BPM key is pressed and hold:
- top row is the hundreds
- second row is the tens
- third row is the units

All patterns are created with a default BPM of 120 which will be represented by the following keys being lit in PURPLE:
- A1 (1 * 100)
- A6 (2 * 10)
- B12 (0 * 1)

In order to set the BPM to a value, do the following:
- Press and hold the BPM key. The current value is displayed.
- Press the keys that correspond to your desired BPM value. For example, to set the tempo to 168, press the keys A1, VEL and COPY.
- Release the BPM key.

To tweak the BPM in realtime, do the following:
- Press and hold the BPM key.
- To decrease BPM by 1, press A3. To increase it by the same value, press A4
- To do likewise 10 by 10, press B1 (decrease) or B2 (increase).
- The BPM value is immediately modified. The new figures for the BPM value are lit in YELLOW. Note that if you set an absolute value, it applies immediately as you do step-wise changes.
- When you are satisfied with the BPM value, release the BPM key and the value will be saved.
- If you want to reset the BPM value, just press the PURPLE keys and release the BPM key.

### Inputting steps

With all the above knowledge now yours, it's time to make a pattern. The process is as follows.

- Select the BANK you want to work on by using the BANK button. The active steps (if any) are lit in the bank's color and the disabled steps (in the case of pages less than 16 steps long) are lit in a dark hue of the current bank color.
- Press the keys that correspond to the steps you want to input. 
- If your pattern is 2 pages long, navigate to the second page and repeat the operation.
- Do the same for other banks.

### Copying steps and patterns

It's useful to copy steps in order to quickly input them.

- You may want to copy steps from your current page to the other page of your current channel.
- You may want to copy steps from your current channel to another channel of the same pattern.
- You may want to copy your current pattern to another pattern slot in order, for example, to make a variant.

To copy the current page to the steps of the other page, do as follows:

- Press and hold the COPY button. 
- For the Zone A channel, press the MUTEA button. For the Zone B channel, press the MUTEB button.
- The button you just pressed flashes 3 times in to indicate that copy was successful.

To copy steps from the current channel to another, do as follows:
- Press and hold the COPY button.
- To copy the Zone A channel, press the LAST button. The COPY and LAST button lights in RED to indicate we are in COPY mode and the current channel button blinks in BLUE (one of A1...A6). You may release the COPY button.
- Select destination channels by pressing buttons in the A1...A6 and B1...B6 ranges. They correspond respectively to Zone A and Zone B, Bank 1 to Bank 6. The pressed buttons blink in GREEN to indicate that they will receive the contents of the currently BLUE-blinking channel.
- To confirm the copy, press the COPY button. It blinks 3 times to confirm.
- To cancel the copy, press the LAST button. All buttons turn off and we exit the copy mode without consequences. 

To copy the current pattern to another pattern slot, do as follows:

- Press and hold the COPY button.
- Press the SEL button. The COPY and SEL button is lit in RED. The current pattern indicator button starts blinking in PURPLE. You may release the COPY button.
- Select destination patterns by pressing buttons in the 2 zones A and B. The pressed buttons blink in ORANGE to indicate they are going to be overwritten with the current pattern.
- To confirm the copy, press the COPY button. It blinks 4 times to confirm once the copy is done.
- To cancel the copy, press the SEL button. All buttons turn off and we exit the copy mode without consequences.

### Playing a pattern

Playing a pattern is as simple as pressing the PLAYSTOP button. It blinks every quarter note as it plays and is off when it doesn't.

When you play a pattern, the key that corresponds to the active step are lit in the Zone A section. Page 1 steps 1-16 are in light blue. Page 2 steps 17-32 are in light green. 

## Song mode operation

SONG mode lets you play patterns one after the other, in realtime and looping.

The general workflow is as follows once patterns are present:

- Set BPM mode (apply song tempo or keep pattern tempo)
- Set Song BPM
- Program sequence of patterns that will play one after the other
- Launch song, control playback, control BPM and channel velocity skew during playback

### Setting BPM mode

There are 3 BPM modes: song tempo, pattern tempo, and external tempo. 

Song tempo entirely ignores the pattern tempo and applies the song's BPM to all patterns alike. Pattern tempo keeps the BPM value of each pattern and honors it. External tempo uses an external clock source to derive a BPM value and applies it strictly to playback.

It's okay to use Song tempo if you do not use large BPM values to squeeze more resolution from the sequencer only in some patterns. A typical example is a trap beat programmed at 160 BPM but meant to feel like a 80 BPM. If you set the Song tempo to 80 BPM, your trap beat programmed for 160 will sound wrong. Song tempo should be fine for most people.

Pattern tempo keeps the BPM value of each pattern. This is nice when all your patterns are consistently rhythmed but quickly becomes odd if you chain patterns with totally different BPM values.

External tempo is useful for performers that rely on an external clock source but limitations apply so use it wisely.

In order to check or set the BPM mode of your song, do the following:

- Press and hold the BPM button. The current mode indicator lights up brightly in GREEN and the others light up dimly: B3 for Song BPM, B7 for Pattern BPM, B11 for External Clock.
- Select the desired BPM mode by pressing one of B3, B7, B11. The corresponding key blinks in GREEN.
- Release the BPM button. The BPM mode is now set.

### Setting Song BPM value

The procedure is identical to the Pattern mode operation:

- Press and hold the BPM button. The current BPM value lights up.
- Input the new BPM value using either absolute settings or real-time changes.
- Release the BPM button.

### Program sequence of patterns

A Song is made of a sequence of patterns. By default, it is empty and we need to populate it.

To append or prepend patterns to a song, do the following:

- Press and hold the SEL key. All the populated pattern keys light up dimly in PURPLE. This means you are in append mode.
- Press the keys that correspond to the patterns you want to append, in order.
- To switch to prepend mode, press the BANK key. The patterns now light up dimly in ORANGE. Press that button again to switch back to append mode.
- Release the SEL button. The pattern buttons light up in play order, each for 400 ms with a 100ms pause between each. This display is interrupted on keypress.

This implies that in order to review the current Song sequence, you may press and release the SEL button.

To reset a song's patterns, do the following:

- Press and hold the SEL key.
- Press the PLAYSTOP key 3 times. Each time, the PLAYSTOP key lights up in RED brighter. The third time, the song patterns are forgotten but the other settings are kept.
- Release the SEL key.

### Reset everything except the pattern sequence

To recover a song's settings and avoid losing the sequence settings, the following may be done:

- Press and hold the VEL key
- Press the PLAYSTOP key 3 times. Each time, it lights up in GREEN brighter. The third time, the song parameters are forgotten.
- Release the VEL key.

### Reset a song entirely

Sometimes it's useful to reinitialize a song entirely, especially after messing up the parameters.

- Press and hold the LAST key.
- Press the PLAYSTOP key 3 times. Each time, it lights up in BLUE brighter. The third time, all song parameters are forgotten and reinitialized.
- Release the LAST key.

### Launch a song and control playback

Launching a song is as simple as pressing the PLAYSTOP button. Pressing it once will play the song from its initial position. Pressing it again will stop the song and rewind it.

To control playback, use the B14, B15, B16 keys:

- B14 restarts the current pattern immediately
- B15 pauses and resumes playback from the current position
- B16 skips to the next pattern in the sequence

The B13 key reverses the playback direction.
- Press it once and it lights up in blue for REW mode. We play each pattern in an inverted manner (last note first, first note last)
- Press it twice and it lights up in orange for WALKUP mode. We play the song in reverse order (each pattern plays back in the right order, but starting from last pattern back to first pattern)
- Press it thrice and it lights up in red, indicating UPSIDE-DOWN mode. We play each pattern in an inverted manner and in reverse order.
- Press it once again and it turns off, resuming normal playback.

When the special B13 modes WALKUP and UPSIDE-DOWN are set, the starting point of playback in case of PLAYSTOP becomes the first step of the last pattern in the sequence for WALKUP and the last step of the last pattern for UPSIDE-DOWN mode.

You may skew the velocity ratio vR. for each of the 12 channels:

- A1/A2:   increase/decrease vR. for Zone A, 1
- B1/B2:   increase/decrease vR. for Zone B, 1
- A3/A4:   increase/decrease vR. for Zone A, 2
- B3/B4:   increase/decrease vR. for Zone B, 2
- A5/A6:   increase/decrease vR. for Zone A, 3
- B5/B6:   increase/decrease vR. for Zone B, 3
- A7/A8:   increase/decrease vR. for Zone A, 4
- B7/B8:   increase/decrease vR. for Zone B, 4
- A9/A10:  increase/decrease vR. for Zone A, 5
- B9/B10:  increase/decrease vR. for Zone B, 5
- A11/A12: increase/decrease vR. for Zone A, 6
- B11/B12: increase/decrease vR. for Zone B, 6 (Mod wheel)

Each skew lights up the corresponding key brighter. If a key pair is not lit at all then its velocity is not skewed.

You may also tweak the playback BPM (except in external clock mode) by applying a tempo ratio tR.

- A13: reset tR.
- A14/A15: decrease/increase tR. by 1%
- A16 + A14/A15: decrease/increase tR. by 10%
- BPM + A14/A15: decrease/increase tR. by 0.1%
