#ifndef SoftwareMicrophone_h
#define SoftwareMicrophone_h

#include "Microphone.h"

class SoftwareMicrophone : public Microphone
{
public:
    void setReading(bool reading);

protected:
    bool read() override;

private:
    bool _reading;
};

#endif