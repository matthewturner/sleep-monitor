#include "Analyzer.h"

Analyzer::Analyzer(TimeProvider *timeProvider)
{
    _timeProvider = timeProvider;
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

void Analyzer::record(bool sound)
{
    if (sound)
    {
        recordSound(_timeProvider->now());
    }
}

void Analyzer::recordSound(unsigned long time)
{
    if (_counter == 0)
    {
        _start = time;
    }

    short index = indexFor(time);
    _samples[index] = true;
}

short Analyzer::indexFor(unsigned long time)
{
    unsigned long timeSinceStart = time - _start;
    short index = (short)(timeSinceStart / SAMPLE_BUFFER_COUNT);
    return index;
}

bool Analyzer::analysisRequired()
{
    if (_counter == 0)
    {
        return false;
    }
    if (_counter >= SAMPLE_BUFFER_COUNT)
    {
        return true;
    }
    if ((_timeProvider->now() - _start) > _durationThreshold)
    {
        return true;
    }
    return false;
}

void Analyzer::analyze(Summary *summary)
{
    summary->Count = 0;
    summary->AverageSoundDuration = 0;
    summary->AverageSilenceDuration = 0;
    summary->TotalSoundDuration = 0;
    summary->TotalSilenceDuration = 0;
    summary->SliceDuration = (short)(_durationThreshold / DISPLAY_LENGTH);

    unsigned short currentSound = 0;
    unsigned short previousSound = 0;
    unsigned short soundCount = 0;
    unsigned short silenceCount = 0;
    unsigned short soundDurations[70];
    unsigned short silenceDurations[70];
    for (unsigned short i = 0; i < _counter; i++)
    {
        currentSound = (i * SLICE_DURATION);
        unsigned long duration = currentSound - previousSound;

        if (previousSound == 0 || duration >= CONTIGUOUS_SOUND_THRESHOLD)
        {
            summary->Count++;
        }

        if (previousSound != 0 && duration < CONTIGUOUS_SOUND_THRESHOLD)
        {
            if (soundCount < MAX_SAMPLE_COUNT)
            {
                summary->TotalSoundDuration += duration;
                soundDurations[soundCount] = duration;
                soundCount++;
            }
        }

        if (previousSound != 0 && duration >= CONTIGUOUS_SOUND_THRESHOLD)
        {
            if (silenceCount < MAX_SAMPLE_COUNT)
            {
                summary->TotalSilenceDuration += duration;
                silenceDurations[silenceCount] = duration;
                silenceCount++;
            }
        }

        previousSound = currentSound;
    }

    unsigned long remainingSilenceDuration = _timeProvider->now() - previousSound;
    if (remainingSilenceDuration > 0)
    {
        summary->TotalSilenceDuration += remainingSilenceDuration;
        silenceDurations[silenceCount] = remainingSilenceDuration;
        silenceCount++;
    }

    summary->AverageSoundDuration = averageSoundDuration(summary, soundCount);
    summary->AverageSilenceDuration = averageSilenceDuration(summary, silenceCount);

    summary->SoundStandardDeviation = standardDeviation(soundDurations, soundCount);
    summary->SilenceStandardDeviation = standardDeviation(silenceDurations, silenceCount);

    summary->Result = determineResult(summary);
    summary->RhythmDetected = rhythmDetected(summary->Result);
}

unsigned long Analyzer::averageSoundDuration(Summary *summary, unsigned short soundCount)
{
    if (soundCount == 0)
    {
        return 0;
    }
    return (unsigned long)(summary->TotalSoundDuration / soundCount);
}

unsigned long Analyzer::averageSilenceDuration(Summary *summary, unsigned short silenceCount)
{
    if (silenceCount == 0)
    {
        return 0;
    }
    return (unsigned long)(summary->TotalSilenceDuration / silenceCount);
}

unsigned short Analyzer::determineResult(Summary *summary)
{
    if (summary->Count < _minSampleThreshold)
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
    for (short i = 0; i < _counter; i++)
    {
        _samples[i] = false;
    }
    _counter = 0;
    _start = 0;
}

short Analyzer::count()
{
    return _counter;
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