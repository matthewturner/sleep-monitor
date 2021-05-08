#ifndef IStreamReader_h
#define IStreamReader_h

class IStreamReader
{
public:
    virtual bool available() = 0;
    virtual char read() = 0;
};

#endif