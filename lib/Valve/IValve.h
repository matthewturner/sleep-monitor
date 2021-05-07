#ifndef IValve_h
#define IValve_h

#define UNDETERMINED 0
#define CLOSED 1
#define OPEN 2

class IValve
{
public:
    virtual void open() = 0;
    virtual void close() = 0;

    virtual char state() = 0;
};

#endif