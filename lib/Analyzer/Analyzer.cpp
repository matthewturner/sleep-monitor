#include "Analyzer.h"

Analyzer::Analyzer()
{
}

void Analyzer::setRhythmThreshold(short newThreshold)
{
    _rhythmThreshold = newThreshold;
}

void Analyzer::recordSilence()
{
    _samples[_counter] = 0;
    _counter++;
}

void Analyzer::recordSound(unsigned long time)
{
    _samples[_counter] = time;
    _counter++;
}

bool Analyzer::analysisRequired()
{
    return (_counter >= SAMPLE_BUFFER_COUNT);
}

bool Analyzer::rhythmicSoundsDetected()
{
    int soundCounter = countRhythmicSounds();
    if (soundCounter >= 5)
    {
        return true;
    }

    return false;
}

int Analyzer::durationRhythmicSounds()
{
    Summary summary;
    analyze(&summary);
    return summary.TotalDuration;
}

int Analyzer::countRhythmicSounds()
{
    Summary summary;
    analyze(&summary);
    return summary.Count;
}

void Analyzer::analyze(Summary* summary)
{
    unsigned long lastSound = 0;
    int soundCounter = 0;
    int totalDuration = 0;
    for (int i = 0; i < _counter; i++)
    {
        unsigned long currentSound = _samples[i];
        if (currentSound == 0)
        {
            continue;
        }

        unsigned long duration = currentSound - lastSound;

        if (lastSound == 0 || duration >= CONTIGUOUS_SILENCE_THRESHOLD)
        {
            soundCounter++;
        }
        
        if (lastSound != 0 && duration < CONTIGUOUS_SILENCE_THRESHOLD)
        {
            totalDuration += duration;
        }
        
        lastSound = currentSound;
    }
    summary->Count = soundCounter;
    summary->TotalDuration = totalDuration;
}

void Analyzer::clear()
{
    _counter = 0;
}