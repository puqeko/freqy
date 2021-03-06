// Freqy.cpp - Analog frequency generation on Arduino Uno.
// https://github.com/puqeko/freqy
// 21-10-17
//
// Use PWM to generate a sqaure wave with a sinusoidally varying duty cycle.
// Putting the signal through a low pass filter will yield a sine wave.

#include <avr/pgmspace.h>
#include <math.h>

#include "freqy.h"
#include "timers.h"

#define PWM_FREQUENCY timer_pwm_frequency(TIMER_PRESCALER_1)

const PROGMEM unsigned char sineTable[] = {
    127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,
    181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,
    225,227,229,231,233,234,236,238,239,240,242,243,244,245,247,248,249,249,
    250,251,252,252,253,253,253,254,254,254,255,254,254,254,253,253,253,252,
    252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,
    229,227,225,223,221,219,217,215,212,210,208,205,203,200,198,195,192,190,
    187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,
    133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,76,
    73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,
    18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,
    5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,33,35,37,39,42,44,46,
    49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,
    115,118,121,124
};
static uint32_t timeStep = (uint32_t)4 << 24;  // about 490 Hz by below calculation
static volatile uint32_t phasePosition = 0;


// Setup registers and initalise values. Called once in setup.
void init_frequency()
{
    // Setup timer 2 as PWM generator
    // TIMER_2 has the ouput compare pin (OC2A) which is pin 11
    // The other pin is OC2B on pin 3.
    pinMode(11, OUTPUT);

    reset_timer2();  // clear timer registers
    timer_set_prescaler(TIMER_2, TIMER_PRESCALER_1);  // as fast as we can

    // Use phase correct PWM with TOP = 0xFF, compare update TOP, overflow when BOTTOM
    // NOTE: Can reduce frequency with different wave mode using OCR2A as TOP and
    // OCR2B as reference. This would output to pin 3 instead of 11.
    // Set timer_set_wavegen_mode with 0x5. However, interupt might run over time.
    timer_set_wavegen_mode(TIMER_2, 0x1);

    // Clear OC2A (output compare pin) on Compare Match when up-counting. Set ouput
    // compare pin A on Compare Match when down-counting.
    // Change of signal (low->high, or high->low) occures when compare value met.
    timer_set_compare_output_mode(TIMER_2, A, 0x2);
    timer_enable_interupt_overflow(TIMER_2);  // enable ISR(TIMER2_OVF_vect) below
}


// When counter value for TIMER_2 is zero (at the bottom), then set the next compare
// value from the sine table.
// This is called at a frequency of 31,372 Hz since 16 MHz / 510 = 31,372 (see datasheet).
ISR(TIMER2_OVF_vect) {
    phasePosition += timeStep;  // clipped automatically on overflow
    timer_set_compare_value(TIMER_2, A, pgm_read_byte_near(&sineTable[phasePosition >> 24]));
}


// Set output frequency
// Use a 32 bit number representing the change in time / x / phase required
// to acheive the desired newFrequency. The top 8 bits represent the true
// value, and the remaining lower bits are used for sub bit precision only.
// Using an integer is required for optimisation of the interupt function.
void set_output_freqy(float newFrequency)
{
    // Rearrange from signalFrequency = (pwmFrequency / 256) * timeStep
    // 256 is the length of the sine table.
    float realTimeStep = 256.0 * newFrequency / PWM_FREQUENCY;

    // Shift value up by 2^24 (i.e. 24 bits) so that we can have
    // sub bit percision when incrimenting time steps. Read off the
    // top 8 bits (32 - 24) to get the realTimeStep back in bits.
    timeStep = (uint32_t)(realTimeStep * pow(2, 24));
}