# Layouts and manual

# Seq8

```
 ,-----------------------------------------------------------------------------------.
 | TgI1 | TgI4 | TgI7 | TgI10| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 1
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | TgI2 | TgI5 | TgI8 | TgI11| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 2
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | TgI3 | TgI6 | TgI9 | TgI12| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 3
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Col1 | Col2 | Col3 | Col4 |      | Switch Mode |      |      |      |      |      | CONTROL LINE
  -----------------------------------------------------------------------------------'
 
 ,----------------------------------.
 | IB1  | IBIn |      |      | SwMod|
 |------+------+------+------+------|
 | IB2  | Stp1 | Stp2 | Stp3 | Stp4 |
 |------+------+------+------+------|
 | IB3  | Stp5 | Stp6 | Stp7 | Stp8 |
 |------+------+------+------+------|
 | IB4  | Stp9 | Stp10| Stp11| Stp12|
 |------+------+------+------+------|
 | NxtP | Stp13| Stp14| Stp15| Stp16|
  ----------------------------------'

```

# With a 4 x 12
To toggle a step for an instrument:
- Press and hold one of Col1...Col4 on the Control Line
- Press one of the StpX keys on either of LINE 1, LINE 2, LINE 3.
- The instrument at the crossing of the column and the line will be set for the given step.

Example: I want to set Stp4 for instrument 9.
- Instrument 9 is on line 3 (TgI9: toggle I9)
- Instrument 9 is on column 4.
- I press and holdt the Col4 button and then press the Stp4 button on line 3.

To toggle an instrument track: press the button corresponding to that instrument.

To see which instruments are set for a given key: the colors indicate which instruments are set.
If a given step on a line has more than one instrument set, its color will alternate between the
colors of instrument tracks it is set for.

# With a 5 x 5

## To address an instrument line

### Select an instrument by buttons

Press IBIn two times in a row to enter instrument select mode. The instruments line will turn deep blue.

Then input an instrument code to select an instrument.

Codes:

- I1: IB1 x 1
- I2: IB1 x 2
- I3: IB1 x 3
- I4: IB2 x 1
- I5: IB2 x 2
- I6: IB2 x 3
- I7: IB3 x 1
- I8: IB3 x 2
- I9: IB3 x 3
- I10 IB4 x 3
- I11 IB4 x 3
- I12 IB4 x 3

Each time you press an IB button, the IB line color will change:
- from deep blue to deep red
- from deep red to green
- from green to purple.

The only the selected instrument button will remain.

To exit selection, press any other Instrument button after starting. Otherwise, after 1 second, the code you selected will be final.

To switch to another instrument, press the IBIn button again and input another code.

To mute the active instrument, press its button. Note that all instruments on the same button will be muted so you can use this to
switch off entire layers of your rhythm at once.

### Select an instrument using MIDI CC

Send MIDI CC 0 with the instrument number (starting from 0x01 for I1 to 0x0C for I12

This instrument remains selected until you select another instrument.

## Seq16

```
 ,-----------------------------------------------------------------------------------.
 | TgI1 | TgI4 | TgI7 | TgI10| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 1
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | TgI2 | TgI5 | TgI8 | TgI11| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 2
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | TgI3 | TgI6 | TgI9 | TgI12| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 3
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Col1 | Col2 | Col3 | Col4 | NxtP | Switch Mode |      |      |      |      |      | CONTROL LINE
  -----------------------------------------------------------------------------------'
 

```

Usage is the same as for Seq8 with one difference: the NxtP button.


### Set steps 9-16
To toggle a step for an instrument, it's the same thing as for Seq8. But in order to set steps
9 to 16, you need to press and hold NextP and ColX before pressing one of the Stp buttons.

### Toggle to/from Page 2

Press NxtP twice in a row, within one second.
Or send Midi CC 3 with value 0x02 to switch to page 2, and 0x01 to switch to page 1.

The NxtP button will change color from green to orange.

# Seq32

32 steps, 8 instruments

Instrument selection is similar to Seq16 but with the following sequences
and max 8 instruments:

Press IBIn two times in a row to enter instrument select mode. The instruments line will turn deep red.

Then input an instrument code to select an instrument.

Codes:

- I1: IB1 x 1
- I2: IB1 x 2
- I3: IB2 x 1
- I4: IB2 x 2
- I5: IB3 x 1
- I6: IB3 x 2
- I7: IB4 x 1
- I8: IB4 x 2

## Change pages and copy content

Not needed on 4x12. On 5x5, press PgB1 to switch to page 1, PgB2 to switch to page 2,
PgB1 + PgB2 to copy the current page to the other page.


```
 ,-----------------------------------------------------------------------------------.
 | Stp1 | Stp2 | Stp3 | Stp4 | Stp17| Stp18| Stp19| Stp20| Stp25| Stp26| Stp27| Stp28|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp5 | Stp6 | Stp7 | Stp8 | Stp21| Stp22| Stp23| Stp24| Stp29| Stp30| Stp31| Stp32|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp9 | Stp10| Stp11| Stp12|      |      |      | PgB1 | Inst1| Inst2| Inst3| Inst4|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp13| Stp14| Stp15| Stp16|      | Switch Mode | PgB2 | Inst5| Inst6| Inst7| Inst8|
  -----------------------------------------------------------------------------------'

 ,----------------------------------.
 | IB1  | IBIn | PgB1 | PgB2 | SwMod|
 |------+------+------+------+------|
 | IB2  | Stp1 | Stp2 | Stp3 | Stp4 |
 |------+------+------+------+------|
 | IB3  | Stp5 | Stp6 | Stp7 | Stp8 |
 |------+------+------+------+------|
 | IB4  | Stp9 | Stp10| Stp11| Stp12|
 |------+------+------+------+------|
 | NxtP | Stp13| Stp14| Stp15| Stp16|
  ----------------------------------'

```



# Seq64

64 steps, 8 instruments. Similar use to Seq32 except that there is twice as much pages.

## Change page and copy contents

### On a 4x12

Use PgB1 to switch to page 1  (steps 1-32)
Use PgB2 to switch to page 2 (steps 33-64)
Use PgB1 + PgB2 to copy the active page to the other page.

When on Page1, the PgB1 button will be lighted up in green.
When on Page2, the PgB2 button will be lighted up in green.

When pattern copy happens, the PgB1 + PgB2 button will flash in orange for 2 seconds.

### On a 5x5

5x5 has 4 pages of 16 steps each.
Use PgB1 to cycle through pages leftward (4->3->2->1->4)
Use PgB2 to cycle through pages righttward (4->3->2->1->4)
Use PgB1 + PgB2 to copy active page to all the other pages.

To know on which page you are, look at the lights:
Page 1: PgB1 in green, PgB2 off
Page 2: PgB1 in red, PgB2 in off
Page 3: PgB1 off, PgB2 in red
Page 4: PgB1 off, PgB2 in green

When copying happens, the PgB1 + PgB2 buttons will flash in orange for 2 seconds.

```
 ,-----------------------------------------------------------------------------------.
 | Stp1 | Stp2 | Stp3 | Stp4 | Stp17| Stp18| Stp19| Stp20| Stp25| Stp26| Stp27| Stp28|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp5 | Stp6 | Stp7 | Stp8 | Stp21| Stp22| Stp23| Stp24| Stp29| Stp30| Stp31| Stp32|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp9 | Stp10| Stp11| Stp12|      |      |      | PgB1 | Inst1| Inst2| Inst3| Inst4|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp13| Stp14| Stp15| Stp16|      | Switch Mode | PgB2 | Inst5| Inst6| Inst7| Inst8|
  -----------------------------------------------------------------------------------'

 ,----------------------------------.
 | IB1  | IBIn | PgB1 | PgB2 | SwMod|
 |------+------+------+------+------|
 | IB2  | Stp1 | Stp2 | Stp3 | Stp4 |
 |------+------+------+------+------|
 | IB3  | Stp5 | Stp6 | Stp7 | Stp8 |
 |------+------+------+------+------|
 | IB4  | Stp9 | Stp10| Stp11| Stp12|
 |------+------+------+------+------|
 |      | Stp13| Stp14| Stp15| Stp16|
  ----------------------------------'

```

# Annex

## Blank layouts

### 4x12 with space bar

```
 ,-----------------------------------------------------------------------------------.
 |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 |      |      |      |      |      |             |      |      |      |      |      |
  -----------------------------------------------------------------------------------'
 
```

### 5x5 ortho
 
```
 ,----------------------------------.
 |      |      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      |      |
  ----------------------------------'

```

## Numbers representation on a 5x5

base-10 numbers, encoded this way. two numbers supported: units in the 4th column
(from left to right) and tens in the 2nd column.

Hundreds are represented by a change in color: if the top LED is red, you are in the
100s. If it is blue, you are in the 200s. No other hundreds are supported.

```
                ONE                                        TWO                                      THREE
 ,----------------------------------.      ,----------------------------------.      ,----------------------------------.
 |      |   X  |      |      |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |      |      |      |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |      |   X  |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
  ----------------------------------'       ----------------------------------'       ----------------------------------'
                FOUR                                       FIVE                                      SIX
 ,----------------------------------.      ,----------------------------------.      ,----------------------------------.
 |      |   X  |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |   X  |      |      |      |      |      |   X  |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |   X  |      |      |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |   X  |      |      |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |      |      |      |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |
  ----------------------------------'       ----------------------------------'       ----------------------------------'
                 SEVEN                                    EIGHT                                     NINE
 ,----------------------------------.      ,----------------------------------.      ,----------------------------------.
 |      |      |      |      |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |      |   X  |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |  X   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|
 |      |  X   |      |      |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |
  ----------------------------------'       ----------------------------------'       ----------------------------------'
                 ZERO
 ,----------------------------------.
 |      |   X  |      |      |      |
 |------+------+------+------+------|
 |      |   X  |      |      |      |
 |------+------+------+------+------|
 |      |   X  |      |      |      |
 |------+------+------+------+------|
 |      |   X  |      |      |      |
 |------+------+------+------+------|
 |      |   X  |      |      |      |
  ----------------------------------'

  Examples
                 TEN                                   TWENTY-FOUR                                FOURTY-SEVEN                 
 ,----------------------------------.      ,----------------------------------.      ,----------------------------------.      
 |      |   X  |      |   X  |      |      |      |   X  |      |   X  |      |      |      |   X  |      |      |      |      
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|      
 |      |      |      |   X  |      |      |      |   X  |      |   X  |      |      |      |   X  |      |      |      |      
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|      
 |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |   X  |      |      |      |      
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|      
 |      |      |      |   X  |      |      |      |      |      |   X  |      |      |      |   X  |      |   X  |      |      
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|      
 |      |      |      |   X  |      |      |      |      |      |      |      |      |      |      |      |   X  |      |      
  ----------------------------------'       ----------------------------------'       ----------------------------------'      
```

