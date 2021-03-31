#include "HardwareStepper.h"

HardwareStepper::HardwareStepper(AccelStepper *stepper)
{
    _stepper = stepper;
}

void HardwareStepper::setSpeed(int speed)
{
    _stepper->setSpeed(speed);
}

void HardwareStepper::run()
{
    _stepper->runSpeed();
}

void HardwareStepper::enable()
{
    _stepper->enableOutputs();
}

void HardwareStepper::disable()
{
    _stepper->disableOutputs();
}

bool HardwareStepper::enabled()
{
    return false;
}
