#ifndef Trigger_h
#define Trigger_h

#include "RuntimeManager.h"

#define DELAY_THRESHOLD 2000

class Trigger
{
public:
    Trigger(RuntimeManager *runtimeManager);

    bool triggered();

protected:
    virtual bool read() = 0;

private:
    RuntimeManager *_runtimeManager;
};

#endif