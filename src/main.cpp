#include "main.h"

#define SOUND_DETECTED LOW

Analyzer analyzer(&Serial);

void setup()
{
  Serial.begin(9600);
  pinMode(7, INPUT);
  Serial.println("Arrgh!!!!!!");
}

bool soundDetected()
{
  int value = digitalRead(7);

  return (value == SOUND_DETECTED);
}

void loop()
{
  Serial.println("Arrgh!!!!!!");

  uint64_t lastSampleTime = millis();
  if (soundDetected())
  {
    analyzer.recordSound(lastSampleTime);
  }
  else
  {
    analyzer.recordSilence();
  }
}