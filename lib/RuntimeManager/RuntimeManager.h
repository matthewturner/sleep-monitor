#ifndef RuntimeManager_h
#define RuntimeManager_h

#define MAX_RUNTIME 20000
#define MIN_WAIT_TIME 10000

class RuntimeManager
{
public:
    void setCurrentTime(unsigned long time);

    void setMaxRuntime(unsigned int maxRuntime);
    void setMinWaitTime(unsigned int minWaitTime);

    bool run();

private:
    unsigned long _currentTime = 0;
    unsigned long _startRuntime = 0;
    unsigned long _lastRuntime = 0;
    unsigned int _maxRuntime = MAX_RUNTIME;
    unsigned int _minWaitTime = MIN_WAIT_TIME;
};

#endif