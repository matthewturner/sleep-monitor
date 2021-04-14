#include "SoftwareTrigger.h"

SoftwareTrigger::SoftwareTrigger(RuntimeManager *runtimeManager)
    : Trigger(runtimeManager)
{
}

bool SoftwareTrigger::read()
{
    return _reading;
}

void SoftwareTrigger::setReading(bool reading)
{
    _reading = reading;
}