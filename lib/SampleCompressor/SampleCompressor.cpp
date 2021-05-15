#include "SampleCompressor.h"

unsigned short SampleCompressor::compress(bool sample)
{
    if (sample == _current)
    {
        _repeat++;
        return 0;
    }
    else
    {
        short repeat = (_repeat == -1 ? 1 : _repeat);
        _repeat = 1;
        _previous = _current;
        _current = sample;
        return repeat;
    }
}

short SampleCompressor::end()
{
    return _repeat;
}

bool SampleCompressor::current()
{
    return _current;
}

bool SampleCompressor::previous()
{
    return _previous;
}

void SampleCompressor::reset()
{
    _previous = false;
    _current = false;
    _repeat = -1;
}