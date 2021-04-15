#ifndef Reporter_h
#define Reporter_h

#include <Arduino.h>

#include "RuntimeManager.h"
#include "Analyzer.h"
#include "Pillow.h"

class Reporter
{
public:
    Reporter(RuntimeManager *runtimeManager);

    void reportOn(Summary *summary);

    void reportOn(Pillow *pillow);

private:
    RuntimeManager *_runtimeManager;
};

#endif