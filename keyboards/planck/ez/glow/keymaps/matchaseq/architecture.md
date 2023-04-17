# Matcha's MIDI Sequencer - technical architecture

This document describes the technical architecture that empowers the sequencer.

## Fundamentals

This is an event-based system. 3 sorts of events can happen:

- user interaction events, for example pressing the COPY key
- time-dependent events, for example timer ticks every 40 ms
- communication events, for example MIDI clock message input, MIDI SysEx input

Events are emitted and passed to an event queue that is then polled by a state machine. The state machine reacts to the sequence of events according to the functional specifications. MIDI commands are also issued as part of an event stream and are issued at a fixed pace.

We want to make this system as reactive as possible, so there shall be as little long processing as possible. The event-based system will allow us to defer execution of changes as much as possible so we do the minimal amount of actual changes.

Events must be processed, and shall not be lost.

In case of state machine incoherence, the event queues and state machine pertaining to that one will be reset.

## User interaction events

User interaction events include for the moment only key events. A key is a binary switch that can be pressed and is then released. Each key is identified by a symbol. Each symbol has a keycode, but there are less keycodes than symbols because keys may have more than one function on space-constrained systems like a 40% keyboard.

KEYPRESS(symbol): symbol has been pushed down
RELEASE(symbol): symbol has been released
KEYTAP(symbol): symbol has been momentarily tapped
CHORDPRESS2(symbol1, symbol2): a 2-symbol chord has been pressed
CHORDRELEASE2(symbol1, symbol2): a 2-symbol chord has been released (both keys have been released)

By issuing these events, it's possible to keep track of the keyboard's state in a way that is not text-input centric.

## Time-dependent events

Time-dependent events are issued periodically.
4 types of such events exist:

TIMERDONE(symbol, wanted_duration, actual_duration, counter): indicate that a timer has completed. counter is set to the value passed at creation + 1
CREATETIMER(symbol, duration, counter): create a timer that will issue a TIMERDONE event after the specified duration has passed.
RENEWTIMER(symbol, duration, counter): re-arm the timer with the specified symbol with given duration and counter.
CANCELTIMER(symbol): cancel the timer with the given symbol.

It's obvious that in order to create a repeating timer, one must issue a RENEWTIMER event.

Timer-related events are processed and issued on an interrupt-based schedule and are broadcast to the appropriate event queues that state machines may poll.

## Communication events

[tbd]


## Data structure

The sequencer has 32 patterns.

Each pattern is made of 11 drum channels and 1 CC channel (for expression).
The properties of a pattern are:
- length (how many steps the pattern has, max 32)
- tempo (what is the tempo in beats per minute, always in quarter notes)
- time division (how long does each of the steps last in terms of note value)

Each channel is made of the same number of steps.
