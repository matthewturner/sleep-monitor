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

void HardwareStepper::setEnablePin(short pin)
{
    _enablePin = pin;
    _stepper->setEnablePin(pin);
}

bool HardwareStepper::enabled()
{
    return (digitalRead(_enablePin) == LOW);
}
