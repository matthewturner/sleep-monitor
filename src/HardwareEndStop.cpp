#include "HardwareEndStop.h"

HardwareEndStop::HardwareEndStop(short pin)
{
    _pin = pin;
    pinMode(_pin, INPUT);
}

bool HardwareEndStop::read()
{
    return digitalRead(_pin) == LOW;
}