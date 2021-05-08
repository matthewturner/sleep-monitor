#include "SoftwareStreamReader.h"

void SoftwareStreamReader::setCommand(const char *command)
{
    _index = 0;
    _command = command;
}

bool SoftwareStreamReader::available()
{
    return *(_command + _index) != '\0';
}

char SoftwareStreamReader::read()
{
    _index++;
    return *(_command + _index - 1);
}