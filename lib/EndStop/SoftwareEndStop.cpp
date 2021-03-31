#include "SoftwareEndStop.h"

bool SoftwareEndStop::read()
{
    return _reading;
}

void SoftwareEndStop::setReading(bool reading)
{
    _reading = reading;
}