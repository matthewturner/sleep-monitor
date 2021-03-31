#include "SoftwareMicrophone.h"

bool SoftwareMicrophone::read()
{
    return _reading;
}

void SoftwareMicrophone::setReading(bool reading)
{
    _reading = reading;
}