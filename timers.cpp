// Timers.cpp
// Help with setting the timer registers on an Arduino Uno.
// https://github.com/puqeko/freqy
// 20-09-17

#include "timers.h"

void reset_timer2()
{
    TCCR2A = TCCR2B = 0;  // Reset control registers.
    TCNT2 = 0;  // Counter value.
    TIFR2 = 0;  // Flag/Event register.
}