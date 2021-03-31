#include "EndStop.h"

EndStop::EndStop()
{
}

bool EndStop::reached()
{
    return read();
}