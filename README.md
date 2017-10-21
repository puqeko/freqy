# Generating a sine wave on the Arduino Uno

Arduino pins are limited to an output of either 0&nbsp;V or 5&nbsp;V. However, we can use PWM and a simple
filter to produce analog signals. Specifically, `freqy.h` uses Timer 2 to generate a PWM wave with
sinusoidally varying duty cycle. Passing the output signal through a low pass filter will yeild a
sine wave signal.

## Setup

Download the repository and open `freqy.ino` with the [Ardunio IDE](https://www.arduino.cc/en/Main/Software).
The example program generates a frequency of 440&nbsp;Hz on pin 11. Use a capacitor and resistor
to construct a simple [low pass filter](http://www.electronics-tutorials.ws/filter/filter_2.html) as shown below.

![Low pass filter](https://user-images.githubusercontent.com/12654833/31846378-59b172f8-b667-11e7-8f96-c55c4cfb1870.png)

Values for C and R could be 3.3&nbsp;nF and 10 k&Omega; respectivly. The idea is to filter out the ~31&nbsp;kHz sqaure wave PWM frequency
leaving the average of the duty cycle. Since the duty cycle is sinusoidally varing, so is the filtered signal. You can
use the formula 

<img width="100" alt="Low pass filter cutoff frequency equation" src="https://user-images.githubusercontent.com/12654833/31846467-addda206-b668-11e7-8d8c-81faa5e98720.png">

to select values for R and C. The cutoff frequency, f<sub>c</sub>, should be somewhere between the PWM frequency and
the desired sine wave output frequency.

Connect the output to a small speaker or view the waveform using an oscilloscope. You may notice small spikes along the waveform at a frequency of
about ~31&nbsp;kHz. This is due to transient switching behaviour on the Arduino. Higher order filters can be used to obtain better results.

## Functions

- `init_frequency()` - This sets up Timer 2 and configures Pin 11 as an output. Call it once at the begining of `setup()`.
- `set_output_freqy(outputFrequency)` - Set the desired sine wave frequency (in Hz). This can be called at any time. The default is around 490&nbsp;Hz.

## Useful Resources

- [Secrets Of Arduino PWM](https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM)
- [Arduion Timers](https://arduino-info.wikispaces.com/Timers-Arduino)
- [ATmega328/P Datasheet](http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf)
