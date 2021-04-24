#ifndef ServoValve_h
#define ServoValve_h

#include "IValve.h"
#include <Servo.h>

#define OPEN_ANGLE 131
#define CLOSED_ANGLE 45

class ServoValve : public IValve
{
public:
    ServoValve(Servo *servo);

    void open() override;
    void close() override;

    char state() override;

private:
    Servo *_servo;
};

#endif