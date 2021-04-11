#ifndef HardwareStepper_h
#define HardwareStepper_h

#include <Arduino.h>
#include "Stepper.h"
#include "RuntimeManager.h"
#include <AccelStepper.h>

class HardwareStepper : public Stepper
{
public:
    HardwareStepper(AccelStepper *stepper, RuntimeManager *runtimeManager);

    void setSpeed(int speed) override;

    void run() override;

    void enable() override;
    void setEnablePin(short pin);

    void disable() override;
    bool enabled() override;

private:
    AccelStepper *_stepper;
    RuntimeManager *_runtimeManager;
    short _enablePin;
};

#endif