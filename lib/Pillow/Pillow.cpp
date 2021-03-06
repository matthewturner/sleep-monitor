#include "Pillow.h"

Pillow::Pillow(EndStop *top, EndStop *bottom, Stepper *stepper, IValve *valve)
{
    _endStopTop = top;
    _endStopBottom = bottom;
    _stepper = stepper;
    _valve = valve;
}

bool Pillow::deflated()
{
    return _endStopTop->reached();
}

bool Pillow::inflated()
{
    return _endStopBottom->reached();
}

short Pillow::intention()
{
    return _action;
}

short Pillow::state()
{
    if (stopped())
    {
        return STOPPED;
    }
    return _action;
}

void Pillow::start(short newAction)
{
    _action = newAction;
    _stepper->setSpeed(_action * 500);
    _stepper->enable();
    _valve->open();
}

bool Pillow::tryInflate()
{
    if (inflated())
    {
        return false;
    }

    start(INFLATING);
    return true;
}

bool Pillow::tryDeflate()
{
    if (deflated())
    {
        return false;
    }

    start(DEFLATING);
    return true;
}

void Pillow::stop()
{
    _stepper->disable();
    if (!deflated())
    {
        _valve->close();
    }
}

bool Pillow::stopped()
{
    return !_stepper->enabled();
}

bool Pillow::running()
{
    return _stepper->enabled();
}

void Pillow::proceed()
{
    if (running())
    {
        _stepper->run();
    }
}

void Pillow::check()
{
    if (deflated())
    {
        if (intention() == DEFLATING)
        {
            stop();
        }
    }

    if (inflated())
    {
        if (intention() == INFLATING)
        {
            stop();
        }
    }
}

void Pillow::reverse()
{
    if (stopped())
    {
        start(intention() * -1);
        return;
    }

    if (deflated())
    {
        start(INFLATING);
        return;
    }

    if (inflated())
    {
        start(DEFLATING);
        return;
    }

    stop();
}