#ifndef Trigger_h
#define Trigger_h

#define DELAY_THRESHOLD 2000

class Trigger
{
public:
    Trigger();

    bool triggered(unsigned long currentTime);

    void setLastTime(unsigned long currentTime);

protected:
    virtual bool read() = 0;

private:
    unsigned long _lastTime;
};

#endif