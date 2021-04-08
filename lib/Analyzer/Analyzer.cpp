#include "Analyzer.h"

Analyzer::Analyzer()
{
}

void Analyzer::setRhythmThreshold(short newThreshold)
{
    _rhythmSampleThreshold = newThreshold;
}

void Analyzer::setSilenceDurationThreshold(short newThreshold)
{
    _silenceDurationThreshold = newThreshold;
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
    if (time - _samples[_counter - 2] > CONTIGUOUS_SILENCE_THRESHOLD)
    {
        _samples[_counter] = time;
        _counter++;
        return;
    }
    // 200 300    n 400 => 200 400
    // overwrite the previous value
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
    if ((time - _samples[0]) > DEFAULT_MAXIMUM_DURATION_THRESHOLD)
    {
        return true;
    }
    return false;
}

void Analyzer::analyze(Summary *summary)
{
    unsigned long previousSound = 0;
    int soundCounter = 0;
    int totalSoundDuration = 0;
    int totalSilenceDuration = 0;
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
            totalSoundDuration += duration;
        }

        if (duration >= CONTIGUOUS_SILENCE_THRESHOLD)
        {
            totalSilenceDuration += duration;
        }

        previousSound = currentSound;
    }
    summary->Count = soundCounter;
    summary->TotalSoundDuration = totalSoundDuration;
    int averageSoundDuration = 0;
    if (soundCounter > 0)
    {
        averageSoundDuration = (int)(totalSoundDuration / soundCounter);
    }
    summary->AverageSoundDuration = averageSoundDuration;

    int averageSilenceDuration = 0;
    if (soundCounter > 1)
    {
        averageSilenceDuration = (int)(totalSilenceDuration / (soundCounter - 1));
    }
    summary->AverageSilenceDuration = averageSilenceDuration;

    bool sampleCountOk = (soundCounter >= _rhythmSampleThreshold);
    bool soundDurationOk = (averageSoundDuration >= _soundDurationThreshold);
    bool silenceDurationOk = (averageSilenceDuration >= _silenceDurationThreshold);
    summary->RhythmDetected = sampleCountOk && soundDurationOk && silenceDurationOk;
}

void Analyzer::clear()
{
    _counter = 0;
}

short Analyzer::count()
{
    return _counter;
}