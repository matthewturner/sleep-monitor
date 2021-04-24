#ifndef SoftwareValve_h
#define SoftwareValve_h

#include "IValve.h"

class SoftwareValve : public IValve
{
public:
    SoftwareValve();

    void open() override;
    void close() override;

    char state() override;

    void reset();

private:
    char _state = OPEN;
};

#endif