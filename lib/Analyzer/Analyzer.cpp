#include "Analyzer.h"

Analyzer::Analyzer()
{
}

void Analyzer::setRhythmThreshold(short newThreshold)
{
    _rhythmThreshold = newThreshold;
}

void Analyzer::setSoundDurationThreshold(short newThreshold)
{
    _soundDurationThreshold = newThreshold;
}

void Analyzer::record(bool sound, unsigned long time)
{
    if (sound)
    {
        recordSound(time);
    }
}

void Analyzer::recordSound(unsigned long time)
{
    if (_counter <= 1)
    {
        _samples[_counter] = time;
        _counter++;
        return;
    }
    if (time - _samples[_counter - 2] > CONTIGUOUS_SILENCE_THRESHOLD)
    {
        _samples[_counter] = time;
        _counter++;
        return;
    }
    _samples[_counter - 1] = time;
}

bool Analyzer::analysisRequired(unsigned long time)
{
    if (_counter == 0)
    {
        return false;
    }
    if (_counter >= SAMPLE_BUFFER_COUNT)
    {
        return true;
    }
    if ((time - _samples[0]) > DEFAULT_DURATION_THRESHOLD)
    {
        return true;
    }
    return false;
}

void Analyzer::analyze(Summary *summary)
{
    unsigned long previousSound = 0;
    int soundCounter = 0;
    int totalDuration = 0;
    for (int i = 0; i < _counter; i++)
    {
        unsigned long currentSound = _samples[i];
        if (currentSound == 0)
        {
            continue;
        }

        unsigned long duration = currentSound - previousSound;

        if (previousSound == 0 || duration >= CONTIGUOUS_SILENCE_THRESHOLD)
        {
            soundCounter++;
        }

        if (previousSound != 0 && duration < CONTIGUOUS_SILENCE_THRESHOLD)
        {
            totalDuration += duration;
        }

        previousSound = currentSound;
    }
    summary->Count = soundCounter;
    summary->TotalDuration = totalDuration;
    int averageDuration = 0;
    if (soundCounter > 0)
    {
        averageDuration = (int)(totalDuration / soundCounter);
    }
    summary->AverageDuration = averageDuration;
    summary->RhythmDetected = (soundCounter >= _rhythmThreshold) && (averageDuration >= _soundDurationThreshold);
}

void Analyzer::clear()
{
    _counter = 0;
}

short Analyzer::count()
{
    return _counter;
}