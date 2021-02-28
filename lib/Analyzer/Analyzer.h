#ifndef Analyzer_h
#define Analyzer_h

#define SAMPLE_BUFFER_COUNT 2000
#define CONTIGUOUS_SILENCE_THRESHOLD 300

class Analyzer
{
public:
    Analyzer();

    void recordSound(unsigned long time);

    void recordSilence();

    bool analysisRequired();

    int countRhythmicSounds();

    bool rhythmicSoundsDetected();

    void clear();

private:
    unsigned long _lastSampleTime = 0;
    unsigned long _samples[SAMPLE_BUFFER_COUNT];
    int _counter = 0;
    bool _rhythmicSoundDetected = false;
};

#endif