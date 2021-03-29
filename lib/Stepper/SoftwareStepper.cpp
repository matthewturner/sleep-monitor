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
