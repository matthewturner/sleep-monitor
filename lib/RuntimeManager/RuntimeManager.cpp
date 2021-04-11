#include "RuntimeManager.h"

RuntimeManager::RuntimeManager(TimeProvider *timeProvider)
{
    _timeProvider = timeProvider;
}

RuntimeManager::RuntimeManager(TimeProvider *timeProvider, unsigned int maxRuntime, unsigned long minWaitTime)
    : RuntimeManager(timeProvider)
{
    _maxRuntime = maxRuntime;
    _minWaitTime = minWaitTime;
}

void RuntimeManager::setCurrentTime(unsigned long time)
{
    _timeProvider->set(time);
}

void RuntimeManager::setMaxRuntime(unsigned int maxRuntime)
{
    _maxRuntime = maxRuntime;
}

void RuntimeManager::setMinWaitTime(unsigned int minWaitTime)
{
    _minWaitTime = minWaitTime;
}

bool RuntimeManager::run()
{
    unsigned long currentTime = _timeProvider->now();
    if (_startRuntime == 0)
    {
        _startRuntime = currentTime;
        _lastRuntime = currentTime;
        return true;
    }
    if ((currentTime - _lastRuntime) > _minWaitTime)
    {
        _startRuntime = currentTime;
        _lastRuntime = currentTime;
        return true;
    }
    if ((_lastRuntime - _startRuntime) < _maxRuntime)
    {
        _lastRuntime = currentTime;
        return true;
    }
    return false;
}

void RuntimeManager::reset()
{
    _startRuntime = 0;
    _lastRuntime = 0;
}

void RuntimeManager::waitFromNow()
{
    unsigned long currentTime = _timeProvider->now();
    _startRuntime = currentTime;
    _lastRuntime = currentTime;
}