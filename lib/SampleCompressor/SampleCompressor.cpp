#include "SampleCompressor.h"

unsigned short SampleCompressor::compress(bool sample)
{
    if (sample == _current)
    {
        _repeat++;
        return 0;
    }

    short repeat = (_repeat == -1 ? 0 : _repeat);
    _repeat = 0;
    _previous = _current;
    _current = sample;
    return repeat;
}

short SampleCompressor::end()
{
    _previous = _current;
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