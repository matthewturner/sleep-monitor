#include <Arduino.h>

#define SOUND_DETECTED LOW
#define SAMPLE_BUFFER_COUNT 5000
#define CONTIGUOUS_SILENCE_THRESHOLD 300

void setup()
{
  Serial.begin(9600);
  pinMode(7, INPUT);
  Serial.println("Arrgh!!!!!!");
}

unsigned long lastSampleTime = 0;
unsigned long samples[SAMPLE_BUFFER_COUNT];
int counter = 0;
bool rhythmicSoundDetected = false;

void analyze()
{
  Serial.println("Analyzing...");

  unsigned long soundStart = 0;
  unsigned long soundEnd = 0;
  unsigned long lastSound = 0;
  bool withinSound = false;

  int silentSampleCounter = 0;
  for (int i = 0; i < SAMPLE_BUFFER_COUNT; i++)
  {
    if (samples[i] == 0)
    {
      silentSampleCounter++;
      continue;
    }

    unsigned long currentSound = samples[i];
    unsigned long potentialSilence = currentSound - lastSound;

    if (potentialSilence >= CONTIGUOUS_SILENCE_THRESHOLD)
    {
      if (withinSound)
      {
        soundEnd = lastSound;
        withinSound = false;
        Serial.print("Sound detected (duration: ");
        Serial.print(soundEnd - soundStart);
        Serial.println("ms)");
      }
      else
      {
        soundStart = lastSound;
        soundEnd = 0;
        withinSound = true;
      }
    }
  }
}

bool soundDetected()
{
  int value = digitalRead(7);

  return (value == SOUND_DETECTED);
}

void loop()
{
  Serial.println("Arrgh!!!!!!");

  lastSampleTime = millis();
  Serial.println(counter);
  if (soundDetected())
  {
    samples[counter] = lastSampleTime;
  }
  else
  {
    samples[counter] = 0;
  }

  counter++;

  Serial.println(counter);
  if (counter >= SAMPLE_BUFFER_COUNT)
  {
    counter = 0;
    analyze();
  }
}