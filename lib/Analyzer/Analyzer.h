#ifndef Analyzer_h
#define Analyzer_h

#include <math.h>
#include "TimeProvider.h"

const char SAMPLE_BUFFER_COUNT = 65;
const short MAX_SAMPLE_BUFFER_COUNT = 2000;
#define DEFAULT_DURATION_THRESHOLD 20000
#define SLICE_DURATION 10
#define SLICE_TO_DISPLAY 20
#define MAX_SAMPLE_COUNT 100
#define CONTIGUOUS_SOUND_THRESHOLD 300
#define DEFAULT_MIN_SAMPLE_THRESHOLD 5
#define DEFAULT_MAX_SAMPLE_THRESHOLD 40
#define DEFAULT_MIN_SOUND_DURATION_THRESHOLD 50
#define DEFAULT_MAX_SOUND_DURATION_THRESHOLD 600
#define DEFAULT_MIN_SILENCE_DURATION_THRESHOLD 2000
#define DEFAULT_MAX_SILENCE_DURATION_THRESHOLD 3000
#define DEFAULT_MAX_SOUND_STANDARD_DEVIATION 300
#define DEFAULT_MAX_SILENCE_STANDARD_DEVIATION 2000

#define DISPLAY_LENGTH 100

#define RHYTHM_DETECTED 0
#define INSUFFICIENT_SAMPLE_COUNT 1
#define INSUFFICIENT_SOUND_DURATION 2
#define EXCESSIVE_SOUND_DURATION 3
#define INSUFFICIENT_SILENCE_DURATION 4
#define EXCESSIVE_SILENCE_DURATION 5
#define EXCESSIVE_SOUND_DEVIATION 6
#define EXCESSIVE_SILENCE_DEVIATION 7
#define UNKNOWN 99

struct summary
{
    unsigned short SoundCount;
    unsigned short SilenceCount;
    unsigned int TotalSoundDuration;
    unsigned int TotalSilenceDuration;
    unsigned int AverageSoundDuration;
    unsigned int AverageSilenceDuration;
    double SoundStandardDeviation;
    double SilenceStandardDeviation;
    unsigned char Result;
    bool RhythmDetected;
    char Display[DISPLAY_LENGTH];
    unsigned short SliceDuration;
    unsigned short SoundDurations[MAX_SAMPLE_COUNT];
    unsigned short SilenceDurations[MAX_SAMPLE_COUNT];
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

    void setSampleThreshold(unsigned short min, unsigned short max);
    void setDurationThreshold(unsigned short threshold);
    void setSoundDurationThreshold(unsigned short min, unsigned short max);
    void setSilenceDurationThreshold(unsigned short min, unsigned short max);

    void setMaxSoundStandardDeviation(unsigned short max);
    void setMaxSilenceStandardDeviation(unsigned short max);

    bool sample(unsigned short index);
    void sample(unsigned short index, bool value);

    void clear();

    short count();

private:
    TimeProvider *_timeProvider;
    bool rhythmDetected(unsigned short status);
    void initialize(Summary *summary);
    void preProcess();
    unsigned short elapsedDuration();
    unsigned short accountedDuration();
    unsigned short determineResult(Summary *summary);
    unsigned int averageSoundDuration(Summary *summary);
    unsigned int averageSilenceDuration(Summary *summary);

    double standardDeviation(unsigned short *samples, unsigned short size);
    double variance(unsigned short *samples, unsigned short size);

    unsigned short indexFor(unsigned long time);
    unsigned short indexForDisplay(unsigned short sliceIndex);
    unsigned char index(unsigned short index);
    unsigned char subIndex(unsigned short index);

    unsigned long _samples[SAMPLE_BUFFER_COUNT];
    unsigned long _start = 0;
    short _index = -1;
    unsigned short _minSampleThreshold = DEFAULT_MIN_SAMPLE_THRESHOLD;
    unsigned short _maxSampleThreshold = DEFAULT_MAX_SAMPLE_THRESHOLD;
    unsigned short _durationThreshold = DEFAULT_DURATION_THRESHOLD;
    unsigned short _minSoundDurationThreshold = DEFAULT_MIN_SOUND_DURATION_THRESHOLD;
    unsigned short _maxSoundDurationThreshold = DEFAULT_MAX_SOUND_DURATION_THRESHOLD;
    unsigned short _minSilenceDurationThreshold = DEFAULT_MIN_SILENCE_DURATION_THRESHOLD;
    unsigned short _maxSilenceDurationThreshold = DEFAULT_MAX_SILENCE_DURATION_THRESHOLD;
    unsigned short _maxSoundStandardDeviation = DEFAULT_MAX_SOUND_STANDARD_DEVIATION;
    unsigned short _maxSilenceStandardDeviation = DEFAULT_MAX_SILENCE_STANDARD_DEVIATION;
};

#endif