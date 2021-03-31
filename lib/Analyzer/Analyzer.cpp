#include "Analyzer.h"

Analyzer::Analyzer()
{
}

void Analyzer::setRhythmThreshold(short newThreshold)
{
    _rhythmThreshold = newThreshold;
}

void Analyzer::record(bool sound, unsigned long time)
{
    if (sound)
    {
        recordSound(time);
    }
    else
    {
        recordSilence(time);
    }
}

void Analyzer::recordSilence(unsigned long time)
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
    if (_counter <= 1)
    {
        return false;
    }
    if (_counter >= SAMPLE_BUFFER_COUNT)
    {
        return true;
    }
    if ((_samples[_counter - 1] - _samples[0]) > DEFAULT_DURATION_THRESHOLD)
    {
        return true;
    }
    return false;
}

void Analyzer::analyze(Summary *summary)
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
    summary->RhythmDetected = (soundCounter >= _rhythmThreshold);
}

void Analyzer::clear()
{
    _counter = 0;
}