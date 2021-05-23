#ifndef Pillow_h
#define Pillow_h

#include "EndStop.h"
#include "Stepper.h"
#include "IValve.h"

#define DEFLATING -1
#define INFLATING 1
#define STOPPED 0

class Pillow
{
public:
    Pillow(EndStop *top, EndStop *bottom, Stepper *stepper, IValve *valve);

    bool deflated();
    bool inflated();

    short intention();
    short state();

    bool tryInflate();
    bool tryDeflate();
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
    IValve *_valve;
    short _action = DEFLATING;
};

#endif