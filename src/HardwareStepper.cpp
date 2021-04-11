#include "HardwareStepper.h"

HardwareStepper::HardwareStepper(AccelStepper *stepper, RuntimeManager *runtimeManager)
{
    _stepper = stepper;
    _runtimeManager = runtimeManager;
}

void HardwareStepper::setSpeed(int speed)
{
    _stepper->setSpeed(speed);
}

void HardwareStepper::run()
{
    if (_runtimeManager->run())
    {
        _stepper->runSpeed();
    }
}

void HardwareStepper::enable()
{
    digitalWrite(_enablePin, LOW);
}

void HardwareStepper::disable()
{
    digitalWrite(_enablePin, HIGH);
}

void HardwareStepper::setEnablePin(short pin)
{
    _enablePin = pin;
    pinMode(pin, OUTPUT);
}

bool HardwareStepper::enabled()
{
    return (digitalRead(_enablePin) == LOW);
}
