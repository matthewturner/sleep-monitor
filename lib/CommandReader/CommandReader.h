#ifndef CommandReader_h
#define CommandReader_h

#include "IStreamReader.h"
#include "string.h"

const unsigned char NONE = 0;
const unsigned char STOP = 1;
const unsigned char INFLATE = 2;
const unsigned char DEFLATE = 3;

class CommandReader
{
public:
    CommandReader(IStreamReader *streamReader);
    unsigned char tryReadCommand();

private:
    unsigned char tryReadInstruction();
    unsigned char convertToCommand(unsigned char instructionLength);
    char _commandBuffer[20];
    IStreamReader *_streamReader;
};

#endif