#include "ServoValve.h"

ServoValve::ServoValve(Servo *servo)
{
    _servo = servo;
}

void ServoValve::open()
{
    _servo->write(OPEN_ANGLE);
}

void ServoValve::close()
{
    _servo->write(CLOSED_ANGLE);
}

char ServoValve::state()
{
    if (_servo->read() == CLOSED_ANGLE)
    {
        return CLOSED;
    }
    return OPEN;
}