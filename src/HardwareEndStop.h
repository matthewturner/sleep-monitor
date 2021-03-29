#ifndef HardwareEndStop_h
#define HardwareEndStop_h

#include "EndStop.h"
#include <Arduino.h>

class HardwareEndStop : public EndStop
{
public:
    HardwareEndStop(short pin);

protected:
    bool read() override;

private:
    short _pin;
};

#endif