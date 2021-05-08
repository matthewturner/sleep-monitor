#include "Analyzer.h"

Analyzer::Analyzer(TimeProvider *timeProvider)
{
    _timeProvider = timeProvider;
}

void Analyzer::analyze(Summary *summary)
{
    initialize(summary);
    preProcess();

    bool withinSound = false;
    bool withinSilence = false;
    for (short i = 0; i <= _index; i++)
    {
        if (_samples[i])
        {
            if (!withinSound)
            {
                withinSound = true;
                withinSilence = false;
                summary->SoundDurations[summary->SoundCount] = 0;
                summary->SoundCount++;
            }
            summary->TotalSoundDuration += SLICE_DURATION;
            summary->SoundDurations[summary->SoundCount - 1] += SLICE_DURATION;
            unsigned short index = indexForDisplay(i);
            summary->Display[index] = '|';
        }
        else
        {
            if (!withinSilence)
            {
                withinSound = false;
                withinSilence = true;
                summary->SilenceDurations[summary->SilenceCount] = 0;
                summary->SilenceCount++;
            }
            summary->TotalSilenceDuration += SLICE_DURATION;
            summary->SilenceDurations[summary->SilenceCount - 1] += SLICE_DURATION;
        }
    }

    if (_index != -1 && _index < SAMPLE_BUFFER_COUNT - 1)
    {
        unsigned short remainingSilenceDuration = elapsedDuration() - accountedDuration();
        if (remainingSilenceDuration > 0 && !withinSilence)
        {
            summary->SilenceDurations[summary->SilenceCount] = 0;
            summary->SilenceCount++;
        }
        summary->SilenceDurations[summary->SilenceCount - 1] = remainingSilenceDuration;
        summary->TotalSilenceDuration += remainingSilenceDuration;
    }

    summary->AverageSoundDuration = averageSoundDuration(summary);
    summary->AverageSilenceDuration = averageSilenceDuration(summary);

    summary->SoundStandardDeviation = standardDeviation(summary->SoundDurations, summary->SoundCount);
    summary->SilenceStandardDeviation = standardDeviation(summary->SilenceDurations, summary->SilenceCount);

    summary->Result = determineResult(summary);
    summary->RhythmDetected = rhythmDetected(summary->Result);
}

bool Analyzer::analysisRequired()
{
    if (_index == -1)
    {
        return false;
    }
    if (_index >= SAMPLE_BUFFER_COUNT)
    {
        return true;
    }
    if (elapsedDuration() > _durationThreshold)
    {
        return true;
    }
    return false;
}

void Analyzer::preProcess()
{
    short indexOfLastSound = -1;
    for (short i = 0; i <= _index; i++)
    {
        if (_samples[i])
        {
            if ((indexOfLastSound != -1) && (i - indexOfLastSound) < (CONTIGUOUS_SOUND_THRESHOLD / SLICE_DURATION))
            {
                // fill in the gap
                for (short j = indexOfLastSound + 1; j <= i; j++)
                {
                    _samples[j] = true;
                }
            }
            indexOfLastSound = i;
        }
    }
}

void Analyzer::initialize(Summary *summary)
{
    summary->SoundCount = 0;
    summary->SilenceCount = 0;
    summary->AverageSoundDuration = 0;
    summary->AverageSilenceDuration = 0;
    summary->TotalSoundDuration = 0;
    summary->TotalSilenceDuration = 0;
    summary->SliceDuration = (short)(_durationThreshold / DISPLAY_LENGTH);
    summary->SoundStandardDeviation = 0;
    summary->SilenceStandardDeviation = 0;
    summary->Result = UNKNOWN;

    for (int i = 0; i < DISPLAY_LENGTH; i++)
    {
        summary->Display[i] = '_';
    }
    for (short i = 0; i < MAX_SAMPLE_COUNT; i++)
    {
        summary->SoundDurations[i] = 0;
        summary->SilenceDurations[i] = 0;
    }
}

void Analyzer::record(bool sound)
{
    if (sound)
    {
        recordSound(_timeProvider->now());
    }
}

void Analyzer::recordSound(unsigned long time)
{
    if (_index == -1)
    {
        _start = time;
    }

    _index = indexFor(time);
    _samples[_index] = true;
}

unsigned short Analyzer::indexForDisplay(unsigned short sliceIndex)
{
    unsigned short indexForDisplay = (unsigned short)(sliceIndex / SLICE_TO_DISPLAY);
    return indexForDisplay;
}

unsigned short Analyzer::indexFor(unsigned long time)
{
    unsigned long timeSinceStart = time - _start;
    unsigned short index = (unsigned short)(timeSinceStart / SLICE_DURATION);
    return index;
}

unsigned short Analyzer::elapsedDuration()
{
    return _timeProvider->now() - _start;
}

unsigned short Analyzer::accountedDuration()
{
    return count() * SLICE_DURATION;
}

unsigned long Analyzer::averageSoundDuration(Summary *summary)
{
    if (summary->SoundCount == 0)
    {
        return 0;
    }
    return (unsigned long)(summary->TotalSoundDuration / summary->SoundCount);
}

unsigned long Analyzer::averageSilenceDuration(Summary *summary)
{
    if (summary->SilenceCount == 0)
    {
        return 0;
    }
    return (unsigned long)(summary->TotalSilenceDuration / summary->SilenceCount);
}

unsigned short Analyzer::determineResult(Summary *summary)
{
    if (summary->SoundCount < _minSampleThreshold)
    {
        return INSUFFICIENT_SAMPLE_COUNT;
    }
    if (summary->AverageSoundDuration < _minSoundDurationThreshold)
    {
        return INSUFFICIENT_SOUND_DURATION;
    }
    if (summary->AverageSoundDuration > _maxSoundDurationThreshold)
    {
        return EXCESSIVE_SOUND_DURATION;
    }
    if (summary->AverageSilenceDuration < _minSilenceDurationThreshold)
    {
        return INSUFFICIENT_SILENCE_DURATION;
    }
    if (summary->AverageSilenceDuration >= _maxSilenceDurationThreshold)
    {
        return EXCESSIVE_SILENCE_DURATION;
    }
    if (summary->SoundStandardDeviation >= _maxSoundStandardDeviation)
    {
        return EXCESSIVE_SOUND_DEVIATION;
    }
    if (summary->SilenceStandardDeviation >= _maxSilenceStandardDeviation)
    {
        return EXCESSIVE_SILENCE_DEVIATION;
    }
    return RHYTHM_DETECTED;
}

void Analyzer::setMaxSoundStandardDeviation(unsigned short max)
{
    _maxSoundStandardDeviation = max;
}

void Analyzer::setMaxSilenceStandardDeviation(unsigned short max)
{
    _maxSilenceStandardDeviation = max;
}

bool Analyzer::rhythmDetected(unsigned short status)
{
    return (status == RHYTHM_DETECTED);
}

void Analyzer::clear()
{
    for (short i = 0; i < SAMPLE_BUFFER_COUNT; i++)
    {
        _samples[i] = false;
    }
    _index = -1;
    _start = 0;
}

short Analyzer::count()
{
    return _index + 1;
}

double Analyzer::standardDeviation(unsigned short *samples, unsigned short size)
{
    if (size <= 1)
    {
        return 0;
    }
    return sqrt(variance(samples, size));
}

double Analyzer::variance(unsigned short *samples, unsigned short size)
{
    if (size <= 1)
    {
        return 0;
    }
    double variance = 0;
    double t = samples[0];
    for (unsigned short i = 1; i < size; i++)
    {
        t += samples[i];
        double diff = ((i + 1) * samples[i]) - t;
        variance += (diff * diff) / ((i + 1.0) * i);
    }

    return variance / (size - 1);
}

void Analyzer::setDurationThreshold(unsigned short threshold)
{
    _durationThreshold = threshold;
}

void Analyzer::setSampleThreshold(unsigned short min, unsigned short max)
{
    _minSampleThreshold = min;
    _maxSampleThreshold = max;
}

void Analyzer::setSilenceDurationThreshold(unsigned short min, unsigned short max)
{
    _minSilenceDurationThreshold = min;
    _maxSilenceDurationThreshold = max;
}

void Analyzer::setSoundDurationThreshold(unsigned short min, unsigned short max)
{
    _minSoundDurationThreshold = min;
    _maxSoundDurationThreshold = max;
}