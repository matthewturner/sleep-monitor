#ifndef Pillow_h
#define Pillow_h

#include "EndStop.h"
#include "Stepper.h"

#define DEFLATING -1
#define INFLATING 1

class Pillow
{
public:
    Pillow(EndStop *top, EndStop *bottom, Stepper *stepper);

    bool deflated();
    bool inflated();

    short intention();

    void start(short newAction);
    void stop();
    bool stopped();

    bool running();
    void proceed();

    void check();
    void reverse();

private:
    EndStop *_endStopTop;
    EndStop *_endStopBottom;
    Stepper *_stepper;
    short _action = DEFLATING;
};

#endif