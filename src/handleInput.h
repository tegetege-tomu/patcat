// #include <Bounce2.h>
#include <stdint.h>
#pragma once

void handleInput();

void handleNotetrigger();
void handleBothcrementPress();

void handleIncrementPress();
void handleDecrementPress();

int handleVelocityPot();
int handleDurationPot();

bool didPotMove(uint8_t pin, int &lastValue, int threshold);
