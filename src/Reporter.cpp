#include "Reporter.h"

Reporter::Reporter(RuntimeManager *runtimeManager)
{
    _runtimeManager = runtimeManager;
}

void Reporter::reportOn(Summary *summary)
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

void Reporter::reportOn(Pillow *pillow)
{
    if (!_runtimeManager->run())
    {
        return;
    }

    if (pillow->inflated())
    {
        Serial.println("Inflated");
    }
    if (pillow->deflated())
    {
        Serial.println("Deflated");
    }

    if (pillow->running())
    {
        if (pillow->intention() == INFLATING)
        {
            Serial.println("Inflating...");
        }
        else
        {
            Serial.println("Deflating...");
        }
    }
}