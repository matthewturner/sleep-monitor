#ifndef Trigger_h
#define Trigger_h

#define DELAY_THRESHOLD 2000

class Trigger
{
public:
    Trigger();

    bool triggered(bool reading, unsigned long currentTime);

    void setLastTime(unsigned long currentTime);

private:
    unsigned long _lastTime;
};

#endif