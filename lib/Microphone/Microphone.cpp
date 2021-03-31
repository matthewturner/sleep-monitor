#include "Microphone.h"

Microphone::Microphone()
{
}

bool Microphone::soundDetected()
{
    return read();
}