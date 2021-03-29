#include "main.h"
#include "HardwareTrigger.h"
#include "HardwareEndStop.h"

Analyzer _analyzer;
HardwareTrigger _trigger(TRIGGER_PIN);
HardwareEndStop _endStopTop(END_STOP_TOP_PIN);
HardwareEndStop _endStopBottom(END_STOP_BOTTOM_PIN);
AccelStepper _stepper = AccelStepper(STEP_INTERFACE_TYPE, STEP_STEP_PIN, STEP_DIRECTION_PIN);
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

  if (fullyDeflated())
  {
    if (deflating())
    {
      stopFlation();
    }
  }

  if (fullyInflated())
  {
    if (inflating())
    {
      stopFlation();
    }
  }

  if (_trigger.triggered(lastSampleTime))
  {
    trigger();
  }

  continueFlating();
}

bool fullyDeflated()
{
  return _endStopTop.reached();
}

bool fullyInflated()
{
  return _endStopBottom.reached();
}

bool inflating()
{
  return (_action == INFLATING);
}

bool deflating()
{
  return (_action == DEFLATING);
}

void startFlation(short newAction)
{
  _action = newAction;
  _stepper.setSpeed(_action * 500);
  digitalWrite(STEP_ENABLE_PIN, LOW);
}

void stopFlation()
{
  digitalWrite(STEP_ENABLE_PIN, HIGH);
}

bool stopped()
{
  return (digitalRead(STEP_ENABLE_PIN) == HIGH);
}

void trigger()
{
  if (fullyDeflated())
  {
    startFlation(INFLATING);
    return;
  }

  if (fullyInflated())
  {
    startFlation(DEFLATING);
    return;
  }

  if (stopped())
  {
    startFlation(_action * -1);
    return;
  }

  stopFlation();
}

void continueFlating()
{
  _stepper.runSpeed();
}