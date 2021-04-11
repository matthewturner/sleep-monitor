#include "RuntimeManager.h"

void RuntimeManager::setCurrentTime(unsigned long time)
{
    _currentTime = time;
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
    if ((_currentTime - _lastRuntime) > _minWaitTime)
    {
        _startRuntime = _currentTime;
        _lastRuntime = _currentTime;
    }
    if ((_lastRuntime - _startRuntime) < _maxRuntime)
    {
        _lastRuntime = _currentTime;
        return true;
    }
    return false;
}