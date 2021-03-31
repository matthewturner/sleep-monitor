#ifndef Stepper_h
#define Stepper_h

class Stepper
{
public:
    Stepper();

    virtual void setSpeed(int speed) = 0;
    virtual void run() = 0;

    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual bool enabled() = 0;
};

#endif