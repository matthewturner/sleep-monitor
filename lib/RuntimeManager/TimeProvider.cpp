#include "TimeProvider.h"

void TimeProvider::set(unsigned long time)
{
    _time = time;
}

unsigned long TimeProvider::now()
{
    return _time;
}