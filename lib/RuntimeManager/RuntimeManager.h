#ifndef RuntimeManager_h
#define RuntimeManager_h

#include "TimeProvider.h"

#define MINUTES 60 * 1000

#define DEFAULT_MAX_RUNTIME 20000
#define DEFAULT_MIN_WAIT_TIME 10000

class RuntimeManager
{
public:
    RuntimeManager(TimeProvider *timeProvider);

    void setCurrentTime(unsigned long time);

    void setMaxRuntime(unsigned int maxRuntime);
    void setMinWaitTime(unsigned int minWaitTime);

    bool run();

    void reset();

    void waitFromNow();

private:
    TimeProvider *_timeProvider;
    unsigned long _startRuntime = 0;
    unsigned long _lastRuntime = 0;
    unsigned int _maxRuntime = DEFAULT_MAX_RUNTIME;
    unsigned int _minWaitTime = DEFAULT_MIN_WAIT_TIME;
};

#endif