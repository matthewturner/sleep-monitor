#include "Pillow.h"

Pillow::Pillow(EndStop *top, EndStop *bottom)
{
    _endStopTop = top;
    _endStopBottom = bottom;
}

bool Pillow::deflated()
{
    return _endStopTop->reached();
}

bool Pillow::inflated()
{
    return _endStopBottom->reached();
}

bool Pillow::inflating()
{
    return (_action == INFLATING);
}

bool Pillow::deflating()
{
    return (_action == DEFLATING);
}

void Pillow::start(short newAction)
{
    _action = newAction;
    //_stepper.setSpeed(_action * 500);
    //digitalWrite(STEP_ENABLE_PIN, LOW);
}

void Pillow::stop()
{
    //digitalWrite(STEP_ENABLE_PIN, HIGH);
}

bool Pillow::stopped()
{
    //return (digitalRead(STEP_ENABLE_PIN) == HIGH);
    return false;
}

void Pillow::proceed()
{
    //_stepper.runSpeed();
}