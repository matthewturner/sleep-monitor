#include "main.h"

AccelStepper _stepper = AccelStepper(STEP_INTERFACE_TYPE, STEP_STEP_PIN, STEP_DIRECTION_PIN);
Analyzer _analyzer;
Summary _summary;
RuntimeManager _runtimeManager;
HardwareTrigger _trigger(TRIGGER_PIN);
HardwareMicrophone _microphone(MICROPHONE_PIN);
HardwareEndStop _endStopTop(END_STOP_TOP_PIN);
HardwareEndStop _endStopBottom(END_STOP_BOTTOM_PIN);
HardwareStepper _stepperAdapter(&_stepper, &_runtimeManager);
Pillow _pillow(&_endStopTop, &_endStopBottom, &_stepperAdapter);
uint64_t _lastOutput;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  _stepperAdapter.setEnablePin(STEP_ENABLE_PIN);
  _stepper.setMaxSpeed(1000);

  _analyzer.setSilenceDurationThreshold(1500, 3000);
  _analyzer.setSoundDurationThreshold(50);

  _pillow.tryDeflate();

  Serial.println("Setup complete. Continuing...");
}

void loop()
{
  uint64_t currentTime = millis();
  _runtimeManager.setCurrentTime(currentTime);

  _pillow.check();

  _analyzer.record(_microphone.soundDetected(), currentTime);

  if (_trigger.triggered(currentTime))
  {
    Serial.println("Triggered!");
    _pillow.reverse();
    _analyzer.clear();
  }

  if (_analyzer.analysisRequired(currentTime))
  {
    Serial.println("Analyzing...");
    _analyzer.analyze(&_summary);
    printSummary(&_summary);
    _analyzer.clear();

    if (_summary.RhythmDetected)
    {
      Serial.println("Rhythm detected");
      _pillow.tryInflate();
    }
  }

  printStatus(currentTime);
  _pillow.proceed();
}

void printSummary(Summary *summary)
{
  Serial.println("Summary");
  Serial.print("\tSample Count: ");
  Serial.println(summary->Count);
  Serial.print("\tTotal Sound Duration: ");
  Serial.println(summary->TotalSoundDuration);
  Serial.print("\tTotal Silence Duration: ");
  Serial.println(summary->TotalSilenceDuration);
  Serial.print("\tAverage Sound Duration: ");
  Serial.println(summary->AverageSoundDuration);
  Serial.print("\tAverage Silence Duration: ");
  Serial.println(summary->AverageSilenceDuration);
  Serial.print("\tRhythm Detected: ");
  Serial.println(summary->RhythmDetected);
}

void printStatus(uint64_t currentTime)
{
  if ((currentTime - _lastOutput) <= 2000)
  {
    return;
  }

  _lastOutput = currentTime;

  if (_pillow.inflated())
  {
    Serial.println("Inflated");
  }
  if (_pillow.deflated())
  {
    Serial.println("Deflated");
  }

  if (_pillow.running())
  {
    if (_pillow.intention() == INFLATING)
    {
      Serial.println("Inflating...");
    }
    else
    {
      Serial.println("Deflating...");
    }
  }
}