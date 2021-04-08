#ifndef Analyzer_h
#define Analyzer_h

#define SAMPLE_BUFFER_COUNT 200
#define CONTIGUOUS_SILENCE_THRESHOLD 300
#define DEFAULT_RHYTHM_THRESHOLD 5
#define DEFAULT_DURATION_THRESHOLD 20000
#define DEFAULT_SOUND_DURATION_THRESHOLD 600

struct summary
{
    short Count;
    short TotalDuration;
    short AverageDuration;
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

    void clear();

    short count();

private:
    unsigned long _samples[SAMPLE_BUFFER_COUNT];
    int _counter = 0;
    short _rhythmThreshold = DEFAULT_RHYTHM_THRESHOLD;
    int _durationThreshold = DEFAULT_DURATION_THRESHOLD;
    short _soundDurationThreshold = DEFAULT_SOUND_DURATION_THRESHOLD;
};

#endif