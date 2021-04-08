#ifndef Analyzer_h
#define Analyzer_h

#define SAMPLE_BUFFER_COUNT 200
#define CONTIGUOUS_SILENCE_THRESHOLD 300
#define DEFAULT_RHYTHM_SAMPLE_THRESHOLD 5
#define DEFAULT_MAXIMUM_DURATION_THRESHOLD 20000
#define DEFAULT_SOUND_DURATION_THRESHOLD 600
#define DEFAULT_SILENCE_DURATION_THRESHOLD 2000

struct summary
{
    short Count;
    int TotalSoundDuration;
    int TotalSilenceDuration;
    int AverageSoundDuration;
    int AverageSilenceDuration;
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
    void setSilenceDurationThreshold(short newThreshold);

    void clear();

    short count();

private:
    bool rhythmDetected(Summary *summary);
    int averageSoundDuration(Summary *summary);
    int averageSilenceDuration(Summary *summary);

    unsigned long _samples[SAMPLE_BUFFER_COUNT];
    int _counter = 0;
    short _rhythmSampleThreshold = DEFAULT_RHYTHM_SAMPLE_THRESHOLD;
    short _durationThreshold = DEFAULT_MAXIMUM_DURATION_THRESHOLD;
    short _soundDurationThreshold = DEFAULT_SOUND_DURATION_THRESHOLD;
    short _silenceDurationThreshold = DEFAULT_SILENCE_DURATION_THRESHOLD;
};

#endif