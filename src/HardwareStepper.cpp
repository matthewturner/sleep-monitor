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
