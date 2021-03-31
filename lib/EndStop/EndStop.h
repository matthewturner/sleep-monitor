#ifndef EndStop_h
#define EndStop_h

class EndStop
{
public:
    EndStop();

    bool reached();

protected:
    virtual bool read() = 0;
};

#endif