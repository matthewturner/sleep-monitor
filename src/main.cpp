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

  _pillow.check();

  if (_trigger.triggered(lastSampleTime))
  {
    _pillow.reverse();
  }

  _pillow.proceed();
}