#include "SoftwareValve.h"

SoftwareValve::SoftwareValve()
{
}

void SoftwareValve::open()
{
    _state = OPEN;
}

void SoftwareValve::close()
{
    _state = CLOSED;
}

char SoftwareValve::state()
{
    return _state;
}

void SoftwareValve::reset()
{
    _state = UNKNOWN;
}