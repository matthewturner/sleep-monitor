#include "Analyzer.h"

Analyzer::Analyzer(TimeProvider *timeProvider)
{
    _timeProvider = timeProvider;
}

void Analyzer::setRhythmThreshold(short newThreshold)
{
    _rhythmSampleThreshold = newThreshold;
}

void Analyzer::setSilenceDurationThreshold(short min, short max)
{
    _minSilenceDurationThreshold = min;
    _maxSilenceDurationThreshold = max;
}

void Analyzer::setSoundDurationThreshold(short newThreshold)
{
    _soundDurationThreshold = newThreshold;
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
    if ((_timeProvider->now() - _samples[0]) > DEFAULT_MAX_DURATION_THRESHOLD)
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
            summary->TotalSoundDuration += duration;
        }

        if (previousSound != 0 && duration >= CONTIGUOUS_SOUND_THRESHOLD)
        {
            summary->TotalSilenceDuration += duration;
        }

        previousSound = currentSound;
    }

    summary->AverageSoundDuration = averageSoundDuration(summary);
    summary->AverageSilenceDuration = averageSilenceDuration(summary);
    summary->RhythmDetected = rhythmDetected(summary);
}

unsigned long Analyzer::averageSoundDuration(Summary *summary)
{
    if (summary->Count == 0)
    {
        return 0;
    }
    return (unsigned long)(summary->TotalSoundDuration / summary->Count);
}

unsigned long Analyzer::averageSilenceDuration(Summary *summary)
{
    if (summary->Count <= 1)
    {
        return 0;
    }
    return (unsigned long)(summary->TotalSilenceDuration / (summary->Count - 1));
}

bool Analyzer::rhythmDetected(Summary *summary)
{
    bool sampleCountOk = (summary->Count >= _rhythmSampleThreshold);
    bool soundDurationOk = (summary->AverageSoundDuration >= _soundDurationThreshold);
    bool silenceDurationOk = (summary->AverageSilenceDuration >= _minSilenceDurationThreshold) && (summary->AverageSilenceDuration < _maxSilenceDurationThreshold);
    return sampleCountOk && soundDurationOk && silenceDurationOk;
}

void Analyzer::clear()
{
    _counter = 0;
}

short Analyzer::count()
{
    return _counter;
}