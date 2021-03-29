#include "main.h"

AccelStepper _stepper = AccelStepper(STEP_INTERFACE_TYPE, STEP_STEP_PIN, STEP_DIRECTION_PIN);
Analyzer _analyzer;
HardwareTrigger _trigger(TRIGGER_PIN);
HardwareEndStop _endStopTop(END_STOP_TOP_PIN);
HardwareEndStop _endStopBottom(END_STOP_BOTTOM_PIN);
Pillow _pillow(&_endStopTop, &_endStopBottom);
short _action = DEFLATING;

void setup()
{
  Serial.begin(9600);
  pinMode(STEP_ENABLE_PIN, OUTPUT);

  _stepper.setMaxSpeed(1000);
}

void loop()
{
  uint64_t lastSampleTime = millis();

  if (_pillow.deflated())
  {
    if (_pillow.deflating())
    {
      _pillow.stop();
    }
  }

  if (_pillow.inflated())
  {
    if (_pillow.inflating())
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
    _pillow.start(_action * -1);
    return;
  }

  _pillow.stop();
}