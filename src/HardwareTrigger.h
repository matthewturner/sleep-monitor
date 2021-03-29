#ifndef HardwareTrigger_h
#define HardwareTrigger_h

#include "Trigger.h"
#include <Arduino.h>

class HardwareTrigger : public Trigger
{
public:
    HardwareTrigger(short pin);

protected:
    bool read() override;

private:
    short _pin;
};

#endif