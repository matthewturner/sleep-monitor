#ifndef Pillow_h
#define Pillow_h

#include "EndStop.h"

#define DEFLATING -1
#define INFLATING 1

class Pillow
{
public:
    Pillow(EndStop *top, EndStop *bottom);

    bool deflated();
    bool inflated();

    bool inflating();
    bool deflating();

    void start(short newAction);
    void stop();
    bool stopped();
    void proceed();

private:
    EndStop *_endStopTop;
    EndStop *_endStopBottom;
    short _action = DEFLATING;
};

#endif