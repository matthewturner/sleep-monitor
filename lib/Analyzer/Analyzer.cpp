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
    if (_counter >= SAMPLE_BUFFER_COUNT)
    {
        return;
    }
    // 200    n xxx => 200 xxx
    if (_counter <= 1)
    {
        _samples[_counter] = time;
        _counter++;
        return;
    }
    // 300 400    n 601 => 300 400 601
    // need to make contiguous sound
    // by maintaining an intervening mark
    if (time - _samples[_counter - 2] > CONTIGUOUS_SOUND_THRESHOLD)
    {
        _samples[_counter] = time;
        _counter++;
        return;
    }
    // 200 300    n 400 => 200 400
    // overwrite the previous value
    _samples[_counter - 1] = time;
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
    if ((_timeProvider->now() - _samples[0]) > _durationThreshold)
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

    for (int i = 0; i < DISPLAY_LENGTH; i++)
    {
        summary->Display[i] = '_';
    }

    unsigned long firstSound = 0;
    summary->SliceDuration = 0;
    if (_counter == 1)
    {
        summary->Display[0] = '|';
    }

    if (_counter > 0)
    {
        firstSound = _samples[0];
        summary->SliceDuration = (short)(_durationThreshold / DISPLAY_LENGTH);
    }
    unsigned long previousSound = 0;
    unsigned short soundCount = 0;
    unsigned short silenceCount = 0;
    unsigned long soundDurations[70];
    unsigned long silenceDurations[70];
    for (unsigned int i = 0; i < _counter; i++)
    {
        unsigned long currentSound = _samples[i];
        if (currentSound == 0)
        {
            continue;
        }

        if (summary->SliceDuration > 0)
        {
            unsigned short displayIndexForSound = (unsigned short)((currentSound - firstSound) / summary->SliceDuration);
            summary->Display[displayIndexForSound] = '|';
        }

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
    _counter = 0;
}

short Analyzer::count()
{
    return _counter;
}

double Analyzer::standardDeviation(unsigned long *samples, unsigned short size)
{
    if (size <= 1)
    {
        return 0;
    }
    return sqrt(variance(samples, size));
}

double Analyzer::variance(unsigned long *samples, unsigned short size)
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