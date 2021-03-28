#include "Trigger.h"

Trigger::Trigger()
{
}

bool Trigger::triggered(unsigned long currentTime)
{
    if (!read())
    {
        return false;
    }

    if (currentTime - _lastTime < DELAY_THRESHOLD)
    {
        return false;
    }

    _lastTime = currentTime;
    return true;
}

void Trigger::setLastTime(unsigned long currentTime)
{
    _lastTime = currentTime;
}