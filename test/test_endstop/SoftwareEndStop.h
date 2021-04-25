#ifndef SoftwareEndStop_h
#define SoftwareEndStop_h

#include "EndStop.h"

class SoftwareEndStop : public EndStop
{
public:
    void setReading(bool reading);

protected:
    bool read() override;

private:
    bool _reading;
};

#endif