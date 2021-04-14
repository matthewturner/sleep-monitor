#include "HardwareTrigger.h"

HardwareTrigger::HardwareTrigger(short pin, RuntimeManager *runtimeManager)
    : Trigger(runtimeManager)
{
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
}

bool HardwareTrigger::read()
{
    return digitalRead(_pin) == LOW;
}