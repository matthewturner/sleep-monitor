#ifndef SoftwareTrigger_h
#define SoftwareTrigger_h

#include "Trigger.h"

class SoftwareTrigger : public Trigger
{
public:
    void setReading(bool reading);

protected:
    bool read() override;

private:
    bool _reading;
};

#endif