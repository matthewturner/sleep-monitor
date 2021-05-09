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
Servo _valveServo;
ServoValve _valve(&_valveServo);
Pillow _pillow(&_endStopTop, &_endStopBottom, &_stepperAdapter, &_valve);
RuntimeManager _summaryRuntimeManager(&_timeProvider, 0, 2000);
Reporter _reporter(&_summaryRuntimeManager);
RuntimeManager _autoInflater(&_timeProvider, 0, INITIAL_AUTO_INFLATE_WAIT_TIME);
HardwareStreamReader _streamReader(&Serial);
CommandReader _commandModule(&_streamReader);

void setup()
{
  Serial.begin(9600);
  Serial.println("Setting up...");

  pinMode(13, OUTPUT);

  _valveServo.attach(VALVE_PIN);

  _stepperAdapter.setEnablePin(STEP_ENABLE_PIN);
  _stepper.setMaxSpeed(1000);

  _analyzer.setSilenceDurationThreshold(1500, 3000);
  _analyzer.setSoundDurationThreshold(50, 1000);

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

  switch (_commandModule.tryReadCommand())
  {
  case STOP:
    Serial.println("Command: Stop");
    _pillow.stop();
    _analyzer.clear();
    _autoInflater.waitFromNow();
    break;
  case INFLATE:
    Serial.println("Command: Inflate");
    _pillow.tryInflate();
    _analyzer.clear();
    _autoInflater.waitFromNow();
    break;
  case DEFLATE:
    Serial.println("Command: Deflate");
    _pillow.tryDeflate();
    _analyzer.clear();
    _autoInflater.waitFromNow();
    break;
  }

  if (_autoInflater.run())
  {
    Serial.println("Auto-inflating...");
    _autoInflater.setMinWaitTime(SUBSEQUENT_AUTO_INFLATE_WAIT_TIME);
    _pillow.tryInflate();
  }

  if (_analyzer.analysisRequired())
  {
    _reporter.reportOn(&_analyzer);
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