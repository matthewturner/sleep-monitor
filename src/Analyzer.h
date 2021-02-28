#ifndef Analyzer_h
#define Analyzer_h

//#include <Arduino.h>

#define SAMPLE_BUFFER_COUNT 2000
#define CONTIGUOUS_SILENCE_THRESHOLD 300

class Analyzer
{
public:
    // Analyzer(HardwareSerial *stream);
    Analyzer();

    void recordSound(uint64_t time);

    void recordSilence();

    bool analyze();

    void analyzeIfRequired();

private:
    //HardwareSerial *_stream;
    uint64_t _lastSampleTime = 0;
    uint64_t _samples[SAMPLE_BUFFER_COUNT];
    int _counter = 0;
    bool _rhythmicSoundDetected = false;
};

#endif