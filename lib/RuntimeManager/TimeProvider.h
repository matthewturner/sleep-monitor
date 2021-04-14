#ifndef TimeProvider_h
#define TimeProvider_h

class TimeProvider
{
public:
    void set(unsigned long time);

    unsigned long now();

private:
    unsigned long _time = 0;
};

#endif