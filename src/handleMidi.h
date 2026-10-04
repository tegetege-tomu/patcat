#include <stdint.h>

#pragma once

void triggerNote(uint8_t n, uint8_t v, uint16_t d);
void noteOn(uint8_t n, uint8_t v);
void noteOff(uint8_t n);
void sendMidi(uint8_t c, uint8_t n, uint8_t v);

/*
void triggerNote();
void noteOn();
void noteOff();
*/
