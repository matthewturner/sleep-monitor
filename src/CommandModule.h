#ifndef CommandModule_h
#define CommandModule_h

#include <Arduino.h>

const unsigned char NONE = 0;
const unsigned char STOP = 1;
const unsigned char INFLATE = 2;
const unsigned char DEFLATE = 3;

class CommandModule
{
public:
    CommandModule(HardwareSerial *stream);
    unsigned char tryReadCommand();
    unsigned char tryReadInstruction();
    unsigned char convertToInstruction(unsigned char commandLength);

private:
    char _commandBuffer[20];
    HardwareSerial *_stream;
};

#endif