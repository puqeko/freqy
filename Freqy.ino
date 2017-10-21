// freqy.ino
// Generate an analog signal using an Arduino Uno.
// https://github.com/puqeko/freqy
// 21-10-17

#include "freqy.h"

void setup()
{
    init_frequency();
    set_output_freqy(440.0);  // Produce 440 Hz
}

void loop() {
    
}