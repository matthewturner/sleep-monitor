#include "CommandModule.h"

CommandModule::CommandModule(HardwareSerial *stream)
{
    _stream = stream;
}

unsigned char CommandModule::tryReadInstruction()
{
    int commandLength = tryReadCommand();
    if (commandLength == 0)
    {
        return -1;
    }
    unsigned char instruction = convertToInstruction(commandLength);
    return instruction;
}

unsigned char CommandModule::tryReadCommand()
{
    int index = -1;
    while (_stream->available())
    {
        delay(2);
        char ch = _stream->read();
        switch (ch)
        {
        case '>':
            index = 0;
            break;
        case '!':
            _commandBuffer[index] = '\0';
            return index + 1;
        default:
            _commandBuffer[index] = ch;
            index++;
            break;
        }
    }
    return 0;
}

unsigned char CommandModule::convertToInstruction(unsigned char commandLength)
{
    if (strcmp(_commandBuffer, "stop") == 0)
    {
        return STOP;
    }
    if (strcmp(_commandBuffer, "inflate") == 0)
    {
        return INFLATE;
    }
    if (strcmp(_commandBuffer, "deflate") == 0)
    {
        return DEFLATE;
    }
    return NONE;
}