#ifndef HardwareMicrophone_h
#define HardwareMicrophone_h

#include "Microphone.h"
#include <Arduino.h>

class HardwareMicrophone : public Microphone
{
public:
    HardwareMicrophone(short pin);

protected:
    bool read() override;

private:
    short _pin;
};

#endif