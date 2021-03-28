#include "SoftwareTrigger.h"

bool SoftwareTrigger::read()
{
    return _reading;
}

void SoftwareTrigger::setReading(bool reading)
{
    _reading = reading;
}