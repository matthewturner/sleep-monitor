#ifndef Analyzer_h
#define Analyzer_h

#include "TimeProvider.h"

#define SAMPLE_BUFFER_COUNT 200
#define CONTIGUOUS_SOUND_THRESHOLD 300
#define DEFAULT_RHYTHM_SAMPLE_THRESHOLD 5
#define DEFAULT_MAX_DURATION_THRESHOLD 20000
#define DEFAULT_SOUND_DURATION_THRESHOLD 600
#define DEFAULT_MIN_SILENCE_DURATION_THRESHOLD 2000
#define DEFAULT_MAX_SILENCE_DURATION_THRESHOLD 3000

#define DISPLAY_LENGTH 100

#define RHYTHM_DETECTED 0
#define INSUFFICIENT_SAMPLE_COUNT 1
#define INSUFFICIENT_SOUND_DURATION 2
#define INSUFFICIENT_SILENCE_DURATION 3
#define EXCESSIVE_SILENCE_DURATION 4

struct summary
{
    unsigned short Count;
    unsigned long TotalSoundDuration;
    unsigned long TotalSilenceDuration;
    unsigned long AverageSoundDuration;
    unsigned long AverageSilenceDuration;
    unsigned short Status;
    bool RhythmDetected;
    char Display[DISPLAY_LENGTH];
    unsigned short SliceDuration;
};

typedef struct summary Summary;

class Analyzer
{
public:
    Analyzer(TimeProvider *timeProvider);

    void record(bool sound);
    void recordSound(unsigned long time);

    bool analysisRequired();

    void analyze(Summary *summary);

    void setRhythmThreshold(short newThreshold);
    void setDurationThreshold(short newThreshold);
    void setSoundDurationThreshold(short newThreshold);
    void setSilenceDurationThreshold(short min, short max);

    void clear();

    short count();

private:
    TimeProvider *_timeProvider;
    bool rhythmDetected(unsigned short status);
    unsigned short determineStatus(Summary *summary);
    unsigned long averageSoundDuration(Summary *summary);
    unsigned long averageSilenceDuration(Summary *summary);

    unsigned long _samples[SAMPLE_BUFFER_COUNT];
    unsigned int _counter = 0;
    unsigned short _rhythmSampleThreshold = DEFAULT_RHYTHM_SAMPLE_THRESHOLD;
    unsigned short _durationThreshold = DEFAULT_MAX_DURATION_THRESHOLD;
    unsigned short _soundDurationThreshold = DEFAULT_SOUND_DURATION_THRESHOLD;
    unsigned short _minSilenceDurationThreshold = DEFAULT_MIN_SILENCE_DURATION_THRESHOLD;
    unsigned short _maxSilenceDurationThreshold = DEFAULT_MAX_SILENCE_DURATION_THRESHOLD;
};

#endif