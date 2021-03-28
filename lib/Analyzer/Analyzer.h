#ifndef Analyzer_h
#define Analyzer_h

#define SAMPLE_BUFFER_COUNT 2000
#define CONTIGUOUS_SILENCE_THRESHOLD 300
#define DEFAULT_RHYTHM_THRESHOLD 5

struct summary
{
    short Count;
    short TotalDuration;
    short AverageDuration;
};

typedef struct summary Summary;

class Analyzer
{
public:
    Analyzer();

    void recordSound(unsigned long time);

    void recordSilence();

    bool analysisRequired();

    int countRhythmicSounds();
    int durationRhythmicSounds();
    void analyze(Summary* summary);

    bool rhythmicSoundsDetected();

    void setRhythmThreshold(short newThreshold);

    void clear();

private:
    unsigned long _samples[SAMPLE_BUFFER_COUNT];
    int _counter = 0;
    short _rhythmThreshold = DEFAULT_RHYTHM_THRESHOLD;
};

#endif