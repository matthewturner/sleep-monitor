#include "main.h"

AccelStepper _stepper = AccelStepper(STEP_INTERFACE_TYPE, STEP_STEP_PIN, STEP_DIRECTION_PIN);
Analyzer _analyzer;
HardwareTrigger _trigger(TRIGGER_PIN);
HardwareEndStop _endStopTop(END_STOP_TOP_PIN);
HardwareEndStop _endStopBottom(END_STOP_BOTTOM_PIN);
HardwareStepper _stepperAdapter(&_stepper);
Pillow _pillow(&_endStopTop, &_endStopBottom, &_stepperAdapter);

void setup()
{
  Serial.begin(9600);

  _stepper.setEnablePin(STEP_ENABLE_PIN);
  _stepper.setMaxSpeed(1000);
}

void loop()
{
  uint64_t lastSampleTime = millis();

  if (_pillow.deflated())
  {
    if (_pillow.intention() == DEFLATING)
    {
      _pillow.stop();
    }
  }

  if (_pillow.inflated())
  {
    if (_pillow.intention() == INFLATING)
    {
      _pillow.stop();
    }
  }

  if (_trigger.triggered(lastSampleTime))
  {
    trigger();
  }

  _pillow.proceed();
}

void trigger()
{
  if (_pillow.deflated())
  {
    _pillow.start(INFLATING);
    return;
  }

  if (_pillow.inflated())
  {
    _pillow.start(DEFLATING);
    return;
  }

  if (_pillow.stopped())
  {
    _pillow.start(_pillow.intention() * -1);
    return;
  }

  _pillow.stop();
}