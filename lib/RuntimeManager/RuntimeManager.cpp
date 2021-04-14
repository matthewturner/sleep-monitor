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
    if (_startRuntime == 0)
    {
        _startRuntime = _currentTime;
        _lastRuntime = _currentTime;
        return true;
    }
    if ((_currentTime - _lastRuntime) > _minWaitTime)
    {
        _startRuntime = _currentTime;
        _lastRuntime = _currentTime;
        return true;
    }
    if ((_lastRuntime - _startRuntime) < _maxRuntime)
    {
        _lastRuntime = _currentTime;
        return true;
    }
    return false;
}

void RuntimeManager::reset()
{
    _startRuntime = 0;
    _lastRuntime = 0;
}