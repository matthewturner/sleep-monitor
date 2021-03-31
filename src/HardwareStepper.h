#ifndef HardwareStepper_h
#define HardwareStepper_h

#include <Arduino.h>
#include "Stepper.h"
#include <AccelStepper.h>

class HardwareStepper : public Stepper
{
public:
    HardwareStepper(AccelStepper *stepper);

    void setSpeed(int speed) override;

    void run() override;

    void enable() override;
    void disable() override;
    bool enabled() override;

private:
    AccelStepper *_stepper;
};

#endif