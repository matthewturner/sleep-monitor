#ifndef ServoValve_h
#define ServoValve_h

#include "IValve.h"
#include <Servo.h>

#define OPEN 1
#define CLOSED 0

#define OPEN_ANGLE 10
#define CLOSED_ANGLE 100

class ServoValve : IValve
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