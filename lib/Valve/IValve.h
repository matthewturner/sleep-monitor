#ifndef IValve_h
#define IValve_h

class IValve
{
public:
    virtual void open() = 0;
    virtual void close() = 0;

    virtual char state() = 0;
};

#endif