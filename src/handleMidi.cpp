
#include "handleMidi.h"
#include <Arduino.h>

void triggerNote(uint8_t n, uint8_t v, uint16_t d) {
  noteOn(n, v);
  digitalWrite(7, LOW);
  delay(d); // CHANGE THIS!! !! !! ASYNCHRONOUS??
  noteOff(n);
  digitalWrite(7, HIGH);
}

void noteOn(uint8_t n, uint8_t v) { sendMidi(0x90, n, v); }

void noteOff(uint8_t n) { sendMidi(0x80, n, 0x0); }

void sendMidi(uint8_t c, uint8_t n, uint8_t v) {
  Serial.write(c);
  Serial.write(n);
  Serial.write(v);
}
