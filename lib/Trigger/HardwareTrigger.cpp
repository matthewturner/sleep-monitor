#include "HardwareTrigger.h"

HardwareTrigger::HardwareTrigger(short pin)
{
    _pin = pin;
}

bool HardwareTrigger::read()
{
    return digitalRead(_pin) == LOW;
}