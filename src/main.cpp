#include "main.h"

AccelStepper _stepper = AccelStepper(STEP_INTERFACE_TYPE, STEP_STEP_PIN, STEP_DIRECTION_PIN);
TimeProvider _timeProvider;
Analyzer _analyzer(&_timeProvider);
Summary _summary;
RuntimeManager _triggerRuntimeManager(&_timeProvider);
HardwareTrigger _trigger(TRIGGER_PIN, &_triggerRuntimeManager);
HardwareMicrophone _microphone(MICROPHONE_PIN);
HardwareEndStop _endStopTop(END_STOP_TOP_PIN);
HardwareEndStop _endStopBottom(END_STOP_BOTTOM_PIN);
RuntimeManager _runtimeManager(&_timeProvider);
HardwareStepper _stepperAdapter(&_stepper, &_runtimeManager);
Pillow _pillow(&_endStopTop, &_endStopBottom, &_stepperAdapter);
RuntimeManager _summaryRuntimeManager(&_timeProvider);

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  _summaryRuntimeManager.setMaxRuntime(0);
  _summaryRuntimeManager.setMinWaitTime(2000);

  _stepperAdapter.setEnablePin(STEP_ENABLE_PIN);
  _stepper.setMaxSpeed(1000);

  _analyzer.setSilenceDurationThreshold(1500, 3000);
  _analyzer.setSoundDurationThreshold(50, 1000);

  _pillow.tryDeflate();

  Serial.println("Setup complete. Continuing...");
}

void loop()
{
  _timeProvider.set(millis());

  _pillow.check();

  _analyzer.record(_microphone.soundDetected());

  if (_trigger.triggered())
  {
    Serial.println("Triggered!");
    _pillow.reverse();
    _analyzer.clear();
  }

  if (_analyzer.analysisRequired())
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

  printStatus();
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
  Serial.println("\tDisplay:");
  Serial.print("\t");
  for (short i = 0; i < DISPLAY_LENGTH; i++)
  {
    if (summary->Display[i] == '|')
    {
      Serial.print('_');
    }
    else
    {
      Serial.print(" ");
    }
  }
  Serial.println();
  Serial.print("\t");
  for (short i = 0; i < DISPLAY_LENGTH; i++)
  {
    if (summary->Display[i] == '_')
    {
      Serial.print('_');
    }
    else
    {
      Serial.print(" ");
    }
  }
  Serial.println();
  unsigned long currentSlice = 0;
  unsigned short time = 5;
  for (short i = 0; i < DISPLAY_LENGTH; i++)
  {
    currentSlice += summary->SliceDuration;
    if ((currentSlice + summary->SliceDuration) % 5000 == 0)
    {
      Serial.print(time);
      time += 5;
    }
    else
    {
      Serial.print(" ");
    }
  }
  Serial.println();
}

void printStatus()
{
  if (!_summaryRuntimeManager.run())
  {
    return;
  }

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