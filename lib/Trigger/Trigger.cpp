#include "Trigger.h"

Trigger::Trigger(RuntimeManager *runtimeManager)
{
    _runtimeManager = runtimeManager;
    _runtimeManager->setMaxRuntime(0);
    _runtimeManager->setMinWaitTime(DELAY_THRESHOLD);
}

bool Trigger::triggered()
{
    if (!read())
    {
        return false;
    }

    return _runtimeManager->run();
}