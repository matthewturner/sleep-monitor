#ifndef SampleCompressor_h
#define SampleCompressor_h

class SampleCompressor
{
public:
    unsigned short compress(bool current);
    short end();
    bool current();
    bool previous();
    void reset();

private:
    bool _current = false;
    bool _previous = false;
    short _repeat = 0;
};

#endif