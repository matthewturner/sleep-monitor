#ifndef SoftwareStepper_h
#define SoftwareStepper_h

#include "Stepper.h"

class SoftwareStepper : public Stepper
{
public:
    void setSpeed(int speed) override;
    int currentSpeed();

    void run() override;

    bool wasRun();
    void reset();

    void enable() override;
    void disable() override;
    bool enabled() override;

private:
    bool _enabled;
    int _speed;
    bool _wasRun;
};

#endif