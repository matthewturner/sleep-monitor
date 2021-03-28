#include "main.h"

#define SOUND_DETECTED LOW
#define END_STOP_TOP_PIN 12
#define END_STOP_BOTTOM_PIN 11
#define TOP_END_STOP END_STOP_TOP_PIN
#define BOTTOM_END_STOP END_STOP_BOTTOM_PIN
#define STEP_INTERFACE_TYPE 1
#define STEP_DIRECTION_PIN 7
#define STEP_STEP_PIN 6
#define STEP_ENABLE_PIN 5
#define MICROPHONE_PIN 3
#define TRIGGER_PIN 2

#define DEFLATING -1
#define INFLATING 1

Analyzer _analyzer;
Trigger _trigger;
AccelStepper _stepper = AccelStepper(STEP_INTERFACE_TYPE, STEP_STEP_PIN, STEP_DIRECTION_PIN);
short _action = DEFLATE;

void setup()
{
  Serial.begin(9600);
  pinMode(MICROPHONE_PIN, INPUT);
  pinMode(END_STOP_TOP, INPUT);
  pinMode(END_STOP_BOTTOM, INPUT);
  pinMode(STEP_ENABLE_PIN, OUTPUT);
  pinMode(MICROPHONE_PIN, INPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

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

  if (_trigger.triggered(triggered(), lastSampleTime))
  {
    trigger();
  }

  continueFlating();

  if (soundDetected())
  {
    _analyzer.recordSound(lastSampleTime);
  }
  else
  {
    _analyzer.recordSilence();
  }
}

bool fullyDeflated()
{
  return reached(TOP_END_STOP);
}

bool fullyInflated()
{
  return reached(BOTTOM_END_STOP);
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

bool triggered()
{
  return (digitalRead(TRIGGER_PIN) == LOW);
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

bool reached(short end)
{
  return digitalRead(end) == HIGH;
}

bool soundDetected()
{
  int value = digitalRead(7);

  return (value == SOUND_DETECTED);
}