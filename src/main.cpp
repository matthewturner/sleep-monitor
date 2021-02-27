#include <Arduino.h>

#define SOUND_DETECTED LOW

void setup()
{
  Serial.begin(9600);
  pinMode(7, INPUT);
}

bool soundStarted = false;
unsigned long lastSound = 0;
unsigned long currentSilence = 0;

void loop()
{
  int value = digitalRead(7);

  if (value == SOUND_DETECTED)
  {
    lastSound = millis();
    if (soundStarted)
    {
      // sound continuing
    }
    else
    {
      Serial.println("Sound started...");
      soundStarted = true;
    }
  }
  else
  {
    if (soundStarted)
    {
      currentSilence = millis();
      if (currentSilence - lastSound > 300)
      {
        Serial.println("Sound finished...");
        soundStarted = false;
      }
    }
    else
    {
      // silence continuing...
    }
  }
}