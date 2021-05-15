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

    void reportOn(Analyzer *analyzer);

    void reportOn(Pillow *pillow);

    void begin();

    void end();

private:
    RuntimeManager *_runtimeManager;
};

#endif