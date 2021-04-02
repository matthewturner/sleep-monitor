#include "main.h"

AccelStepper _stepper = AccelStepper(STEP_INTERFACE_TYPE, STEP_STEP_PIN, STEP_DIRECTION_PIN);
Analyzer _analyzer;
Summary _summary;
HardwareTrigger _trigger(TRIGGER_PIN);
HardwareMicrophone _microphone(MICROPHONE_PIN);
HardwareEndStop _endStopTop(END_STOP_TOP_PIN);
HardwareEndStop _endStopBottom(END_STOP_BOTTOM_PIN);
HardwareStepper _stepperAdapter(&_stepper);
Pillow _pillow(&_endStopTop, &_endStopBottom, &_stepperAdapter);

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  _stepperAdapter.setEnablePin(STEP_ENABLE_PIN);
  _stepper.setMaxSpeed(1000);

  _pillow.tryDeflate();
}

void loop()
{
  uint64_t currentTime = millis();

  _pillow.check();

  _analyzer.record(_microphone.soundDetected(), currentTime);

  if (_trigger.triggered(currentTime))
  {
    Serial.println("Triggered!");
    if (digitalRead(13) == HIGH)
    {
      digitalWrite(13, LOW);
    }
    else
    {
      digitalWrite(13, HIGH);
    }
    _pillow.reverse();
    _analyzer.clear();
  }

  if (_analyzer.analysisRequired(currentTime))
  {
    _analyzer.analyze(&_summary);
    _analyzer.clear();

    if (_summary.RhythmDetected)
    {
      _pillow.tryInflate();
    }
  }

  Serial.println("Proceeding...");
  _pillow.proceed();
}