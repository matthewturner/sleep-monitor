#include "HardwareEndStop.h"

HardwareEndStop::HardwareEndStop(short pin)
{
    _pin = pin;
}

bool HardwareEndStop::read()
{
    return digitalRead(_pin) == HIGH;
}