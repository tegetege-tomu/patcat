// based on Based on Sketch built by Gustavo Silveira (aka Music Nerd) and Dolce
// Wang
// PRELIM WORK COPIED FROM MY OWN MIDIBUDDY!

/*
setup: uno with midi shield (sparkfun)
*/

// LIBRARY
// #include <EEPROM.h> //* need to include these in ino file too for linking
// #include <Bounce2.h>
// #include "Arduino.h"
#include "src/handleInput.h"
#include "src/settings.h"
// #include <MIDI.h> no need since using serial directly

// DEBOUNCE?

// INIT BUTTON AND POTS

// MIDI assignments
/*
const byte MIDI_CH = 0; //* which MIDI channel is used
const byte NOTE = 36;   //* C2
const byte VELOCITY = 100;
const int DURATION_MS = 120;
*/

void initAll();

// SETUP
void setup() {
  // Baud Rate
  // 31250 for MIDI class compliant
  Serial.begin(31250);
  // other serial output here for debug only
  // Serial.println("serial comm started");
  initAll();
  // Serial.println("initAlled");
  // Serial.println("settings set... note: %d, vel: %d, dur: %d ",
  // settings.note,
  //                settings.velocity, settings.durationMs);
}

// LOOP
void loop() {
  handleInput();
  // other serial output here for debug only?
}

// INIT
void initAll() {
  // if statement for loading from persistence
  // anything else needs to be done for midi? serial baud rate already set
  //
  // whatever needs to be done for the resistance re buttons and pots
  pinMode(2, INPUT_PULLUP); // DIN-top: right-most, increment
  pinMode(3, INPUT_PULLUP); // centre: note trigger
  pinMode(4, INPUT_PULLUP); // centre: left-most, decrement
  // pots no need set; LHS = 0 - velocity, RHS = 1 - duration;
  pinMode(6, OUTPUT);    // green LED
  digitalWrite(6, HIGH); // LED off
  pinMode(7, OUTPUT);    // red LED
  digitalWrite(7, HIGH); // LED off
}
