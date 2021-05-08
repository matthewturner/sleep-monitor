#include "HardwareStreamReader.h"

HardwareStreamReader::HardwareStreamReader(HardwareSerial *stream)
{
    _stream = stream;
}

bool HardwareStreamReader::available()
{
    return _stream->available();
}

char HardwareStreamReader::read()
{
    delay(2);
    return _stream->read();
}