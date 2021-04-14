#ifndef HardwareTrigger_h
#define HardwareTrigger_h

#include "Trigger.h"
#include <Arduino.h>

class HardwareTrigger : public Trigger
{
public:
    HardwareTrigger(short pin, RuntimeManager *runtimeManager);

protected:
    bool read() override;

private:
    short _pin;
};

#endif