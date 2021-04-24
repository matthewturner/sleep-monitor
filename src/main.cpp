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
RuntimeManager _summaryRuntimeManager(&_timeProvider, 0, 2000);
Reporter _reporter(&_summaryRuntimeManager);
RuntimeManager _autoInflater(&_timeProvider, 0, INITIAL_AUTO_INFLATE_WAIT_TIME);
Servo _valveServo;
ServoValve _valve(&_valveServo);

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  _valveServo.attach(VALVE_PIN);

  _stepperAdapter.setEnablePin(STEP_ENABLE_PIN);
  _stepper.setMaxSpeed(1000);

  _analyzer.setSilenceDurationThreshold(1500, 3000);
  _analyzer.setSoundDurationThreshold(50, 1000);

  _valve.open();
  _pillow.tryDeflate();

  delay(10);
  _timeProvider.set(millis());
  _autoInflater.waitFromNow();

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
    _autoInflater.waitFromNow();
  }

  if (_autoInflater.run())
  {
    Serial.println("Auto-inflating...");
    _autoInflater.setMinWaitTime(SUBSEQUENT_AUTO_INFLATE_WAIT_TIME);
    _pillow.tryInflate();
  }

  if (_analyzer.analysisRequired())
  {
    Serial.println("Analyzing...");
    _analyzer.analyze(&_summary);
    _reporter.reportOn(&_summary);
    _analyzer.clear();

    if (_summary.RhythmDetected)
    {
      Serial.println("Rhythm detected");
      _pillow.tryInflate();
    }
  }

  _reporter.reportOn(&_pillow);
  _pillow.proceed();
}