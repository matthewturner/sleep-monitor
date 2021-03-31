#include "SoftwareStepper.h"

void SoftwareStepper::setSpeed(int speed)
{
    _speed = speed;
}

int SoftwareStepper::currentSpeed()
{
    return _speed;
}

void SoftwareStepper::run()
{
    _wasRun = true;
}

bool SoftwareStepper::wasRun()
{
    return _wasRun;
}

void SoftwareStepper::reset()
{
    _wasRun = false;
}

void SoftwareStepper::enable()
{
    _enabled = true;
}

void SoftwareStepper::disable()
{
    _enabled = false;
}

bool SoftwareStepper::enabled()
{
    return _enabled;
}
