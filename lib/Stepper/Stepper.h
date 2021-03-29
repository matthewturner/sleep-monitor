#ifndef Stepper_h
#define Stepper_h

class Stepper
{
public:
    Stepper();

    virtual bool setSpeed(int speed) = 0;
    virtual bool run() = 0;

    virtual void enable() = 0;
    virtual void disable() = 0;
};

#endif