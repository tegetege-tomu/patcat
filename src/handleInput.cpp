#include "handleInput.h"
#include "handleMidi.h"
#include "settings.h"
#include <Arduino.h>
#include <Bounce2.h>

void handleInput() {
  handleNotetrigger();
  handleBothcrementPress();
  if (settings.mode) {
    handleIncrementPress();
    handleDecrementPress();
    handleVelocityPot();
    handleDurationPot();
  }
}

void handleNotetrigger() {
  if ((digitalRead(3) == LOW) && !settings.mode) {
    triggerNote(settings.note, settings.velocity, settings.durationMs);
    // Serial.println("note triggered");
  }
  if ((digitalRead(3) == LOW) && settings.mode) {
    settings.mode = false;
    // Serial.println("returned to play mode");
  }
}

void handleBothcrementPress() {
  if ((digitalRead(2) == LOW) && (digitalRead(4) == LOW) && !settings.mode) {
    settings.mode = true;
    // Serial.println("entered settings mode");
  }
}

void handleIncrementPress() {
  if ((digitalRead(2) == LOW) && settings.mode) {
    settings.note = settings.note + 1;
  }
}

void handleDecrementPress() {
  if ((digitalRead(4) == LOW) && settings.mode) {
    settings.note = settings.note - 1;
  }
}

int handleVelocityPot();
int handleDurationPot();

bool didPotMove(uint8_t pin, int &lastValue, int threshold);
