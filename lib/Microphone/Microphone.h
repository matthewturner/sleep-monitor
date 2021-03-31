#ifndef Microphone_h
#define Microphone_h

class Microphone
{
public:
    Microphone();

    bool soundDetected();

protected:
    virtual bool read() = 0;
};

#endif