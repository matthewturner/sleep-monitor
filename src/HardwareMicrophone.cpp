#include "HardwareMicrophone.h"

HardwareMicrophone::HardwareMicrophone(short pin)
{
    _pin = pin;
    pinMode(_pin, INPUT);
}

bool HardwareMicrophone::read()
{
    return digitalRead(_pin) == HIGH;
}