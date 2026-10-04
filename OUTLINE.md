# OUTLINE: some design specs and choice

choices:
Shield config: hardware serial
MIDI jack mode: MIDI OUT
Power over MIDI: disabled

## FILES

.ino will just sit there for setup and serial init.

## BUTTONS

enable/ disable patcat - chord of 1 & 3, long-press

enter/ leave settings - chord of 2 & 3, long-press

toggle sustain on/ off - button 1, long-press: AT sustain; AT independent

toggle sustain type - chord of 1 & 2: AT max; AT lock

toggle note thru - button 2, long-press: AT only sent over midi; AT + note thru

toggle trigger mode - button 3, long-press: zero AT at note-on; current AT at note-on

MIDI panic (quick) - button 3, short-press: send all notes off, all AT off

MIDI panic (full) - button 3, long-press: force all notes off (long cycle)

## ARCHITECTURE

```
MIDI IN
  |
  ↓
Parser (FSM) 
  |
  ↓
Router ─┬-→ pass-through queue ─┐
        │                       ├-→  TxScheduler -→  Writer (running status) -→ MIDI OUT
        └-→ Translator ─────────┘
                  │
                  └─ HeldNotes set, pending pressure
```

## HARDWARE

via https://learn.sparkfun.com/tutorials/midi-shield-hookup-guide

### Uno + MIDI shield

*(jpeg photo of each before everything is attached would be nice)*

```
Uno R3 + SparkFun MIDI Shield v1.5, top view
              SCL ... D13 .... D8    D7 ......... D0
 +-----------[o o o o o o o o o o]--[o o o o o o o o]-------+
 |                                                          |
+------+    +----------+  +----------+                      |
|USB-B |    | MIDI IN  |  | MIDI OUT |     (G) LED D6       |
+------+    |   .--.   |  |   .--.   |     (R) LED D7       |
 |          |  ( oo )  |  |  ( oo )  |                      |
 |          |   '--'   |  |   '--'   |                      |
 |          +----------+  +----------+                      |
 |          [RUN|PROG] slide switch                         |
 |                                                          |
+------+                                   .-.     .-.      |
|  DC  |    [ D2 ]  [ D3 ]  [ D4 ]        (A0 )   (A1 )     |
| jack |      B1      B2      B3           '-'     '-'      |
+------+     TRIG    SUS     PANIC         pot     pot      |
 |                                                          |
 |                                                          |
 +------------[o o o o o o o o]-----[o o o o o o]-----------+
              power header           A0 ..... A5
 USB-B and DC jack are from Uno underneath
```

### Pinout

```
+--------+-----------------+-----------------+----------------+
| Pin    | Shield part     | Electrical      | at2pat role    |
+========+=================+=================+================+
| D0     | MIDI IN         | UART RX; only   | MIDI in        |
|        | (opto-isolated) | in RUN          |                |
+--------+-----------------+-----------------+----------------+
| D1     | MIDI OUT        | UART TX         | MIDI out       |
|        | (buffered)      |                 |                |
+--------+-----------------+-----------------+----------------+
| D2     | B1 push button  | INPUT_PULLUP,   | ON/OFF chord;  |
|        |                 | LOW = pressed   | SUST toggle    |
+--------+-----------------+-----------------+----------------+
| D3     | B2 push button  | INPUT_PULLUP,   | SETTINGS chord;|
|        |                 | LOW = pressed   | THRU toggle    |
+--------+-----------------+-----------------+----------------+
| D4     | B3 push button  | INPUT_PULLUP,   | PANIC (short,  |
|        |                 | LOW = pressed   | long); TRIG;   |
|        |                 |                 | main chord     |
+--------+-----------------+-----------------+----------------+
| D6     | LED green       | OUTPUT, LOW =   | ready,         |
|        |                 | on              | activity,      |
|        |                 |                 | value blinks   |
+--------+-----------------+-----------------+----------------+
| D7     | LED red         | OUTPUT, LOW =   | FUNC mode,     |
|        |                 | on              | panic          |
+--------+-----------------+-----------------+----------------+
| A0     | pot 10k         | analogRead      | reserved       |
|        |                 |                 | (stretch)      |
+--------+-----------------+-----------------+----------------+
| A1     | pot 10k         | analogRead      | reserved       |
|        |                 |                 | (stretch)      |
+--------+-----------------+-----------------+----------------+
| D8, D9 | soft serial     | unused          | --             |
|        | (via SJ1/SJ2)   |                 |                |
+--------+-----------------+-----------------+----------------+
```

NO MODIFICATIONS RE PHYSICAL JUMPERS, SOFT SERIAL

```

+--------------+------------------+----------------------------+
| Switch /     | Setting          | Why                        |
| jumper       |                  |                            |
+==============+==================+============================+
| RUN/PROG     | PROG to upload,  | MIDI IN and the USB        |
| switch, next | RUN to play      | bootloader share D0        |
| to MIDI IN   |                  |                            |
+--------------+------------------+----------------------------+
| SJ1, SJ2     | default: HW      | soft serial on D8/D9 frees |
| (bottom,     | serial on D0/D1  | USB Serial for debug       |
| near         |                  | prints; only AltSoftSerial |
| D10-D13)     |                  | (RX 8, TX 9) is viable at  |
|              |                  | 31250, see D19             |
+--------------+------------------+----------------------------+
| SJ3 (bottom, | default: OUT     | THRU would bypass the      |
| center)      |                  | firmware                   |
+--------------+------------------+----------------------------+
| SJ4-SJ7      | leave open       | power over MIDI is not     |
| (under the   |                  | standard                   |
| jacks)       |                  |                            |
+--------------+------------------+----------------------------+
```

### Buttons and LEDs

these are "active-low"

```
 BUTTON  B1..B3 on D2, D3, D4     LED  green D6, red D7

     +5V                              +5V
      |                                |
     [R]  internal pull-up            [R]  series resistor
      |   20-50 k (INPUT_PULLUP)       |
      +------> Dx reads                |
      |        HIGH = released        _|_
     _|_       LOW  = pressed         \ /  LED
     o o  push button                 -+-
      |                                |
     GND                               +------< Dx writes
                                               HIGH = off
                                               LOW  = on
```

### HW Signal path

```
 controller            at2pat box
+----------+    +-----------------------------+
| MIDI OUT |--->| MIDI IN                     |
+----------+    |  -> opto-isolator           |
                |  -> RUN/PROG switch (RUN)   |
                |  -> D0/RX, UART, RX ring    |
                |  -> firmware (catpat)       |
                |  -> TX ring, UART, D1/TX    |      synth
                |  -> output buffer           |    +---------+
                | MIDI OUT                    |--->| MIDI IN |
                +-----------------------------+    +---------+
```

```
 MIDI 1.0 current loop (5 V)

 SENDER: shield MIDI OUT             RECEIVER: synth MIDI IN

                  pin 4             pin 4
 +5V --[220R]------o=================o-----+
                                           |
                                          _|_  opto LED
                                          \ /  (+ series R,
                                          -+-   reverse diode)
                  pin 5             pin 5  |
 TX ---[buf]-[220R]o=================o-----+

 pin 2 = cable shield, grounded at the sender only
```

```
 TX = 1 (idle, stop bit)  -> no loop current -> RX = 1
 TX = 0 (start, 0 bits)   -> ~5 mA flows     -> RX = 0
 RX mirrors TX; no shared ground, no return path, no ACK:
 MIDI 1.0 is fire-and-forget, so a lost NoteOff is silent
```
MIDI BYTES FOR REFERENCE
and timing math
```
 0xD0 is MIDI "Channel Pressure" Aftertouch
```
"8N1" protocol:

- 1 Start bit, 8 Data bits, No parity bit, 1 Stop bit
- LSB first
- 31250 baud
- 32 us/bit
- 10 bits per byte
- 1 MIDI byte takes 32us

ENDIANNESS?? FLIPPEDEDNESS???

- Binary (MSB to LSB): 1101 0000
- Flipped for transmission (LSB to MSB): 0000 1011

```
St = Start
b0..7 = data
b0 = LSB
b7 = MSB

 bit   idl  St  b0  b1  b2  b3  b4  b5  b6  b7  Sp idl
 val     1   0   0   0   0   0   1   0   1   1   1   1
 TX    ___                     ___     _______________
          |___________________|   |___|

 0xD_ (upper nibble) = Channel Pressure command
 _0 (lower nibble) = MIDI Channel 1 (i.e. CH0 in binary)

 1 start + 8 data + 1 stop = 10 bits = 320 us per byte
 at most 3125 bytes/s in each direction
```
