#ifndef HardwareStreamReader_h
#define HardwareStreamReader_h

#include <Arduino.h>
#include "IStreamReader.h"

class HardwareStreamReader : public IStreamReader
{
public:
    HardwareStreamReader(HardwareSerial *stream);
    bool available() override;
    char read() override;

private:
    HardwareSerial *_stream;
};

#endif