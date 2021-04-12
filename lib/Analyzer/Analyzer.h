#ifndef Analyzer_h
#define Analyzer_h

#define SAMPLE_BUFFER_COUNT 200
#define CONTIGUOUS_SOUND_THRESHOLD 300
#define DEFAULT_RHYTHM_SAMPLE_THRESHOLD 5
#define DEFAULT_MAX_DURATION_THRESHOLD 20000
#define DEFAULT_SOUND_DURATION_THRESHOLD 600
#define DEFAULT_MIN_SILENCE_DURATION_THRESHOLD 2000
#define DEFAULT_MAX_SILENCE_DURATION_THRESHOLD 3000

struct summary
{
    unsigned short Count;
    unsigned long TotalSoundDuration;
    unsigned long TotalSilenceDuration;
    unsigned long AverageSoundDuration;
    unsigned long AverageSilenceDuration;
    bool RhythmDetected;
};

typedef struct summary Summary;

class Analyzer
{
public:
    Analyzer();

    void record(bool sound, unsigned long time);
    void recordSound(unsigned long time);

    bool analysisRequired(unsigned long time);

    void analyze(Summary *summary);

    void setRhythmThreshold(short newThreshold);
    void setSoundDurationThreshold(short newThreshold);
    void setSilenceDurationThreshold(short min, short max);

    void clear();

    short count();

private:
    bool rhythmDetected(Summary *summary);
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