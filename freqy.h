// Freqy.h - Analog frequency generation on Arduino Uno.
// https://github.com/puqeko/freqy
// 21-10-17

#include "Arduino.h"

#ifndef FREQ_H
#define FREQ_H

// Setup timers and pin 11 as output.
void init_frequency();

// Set pin 11 to generate a PWM wave with a duty cycle that varies
// sinusoidally at 'newFrequency' Hz.
void set_output_freqy(float newFrequency);

#endif