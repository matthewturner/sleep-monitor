#include "Analyzer.h"

// Analyzer::Analyzer(HardwareSerial *stream)
// {
//     _stream = stream;
// }

Analyzer::Analyzer()
{
}

void Analyzer::recordSilence()
{
    _samples[_counter] = 0;
    _counter++;
    analyzeIfRequired();
}

void Analyzer::recordSound(uint64_t time)
{
    _samples[_counter] = time;
    _counter++;
    analyzeIfRequired();
}

void Analyzer::analyzeIfRequired()
{
    if (_counter >= SAMPLE_BUFFER_COUNT)
    {
        _counter = 0;
        analyze();
    }
}

bool Analyzer::analyze()
{
    return false;
    // _stream->println("Analyzing...");

    // uint64_t soundStart = 0;
    // uint64_t soundEnd = 0;
    // uint64_t lastSound = 0;
    // bool withinSound = false;

    // int silentSampleCounter = 0;
    // for (int i = 0; i < SAMPLE_BUFFER_COUNT; i++)
    // {
    //     if (_samples[i] == 0)
    //     {
    //         silentSampleCounter++;
    //         continue;
    //     }

    //     uint64_t currentSound = _samples[i];
    //     uint64_t potentialSilence = currentSound - lastSound;

    //     if (potentialSilence >= CONTIGUOUS_SILENCE_THRESHOLD)
    //     {
    //         if (withinSound)
    //         {
    //             soundEnd = lastSound;
    //             withinSound = false;
    //             _stream->print("Sound detected (duration: ");
    //             //_stream->print((uint64_t)(soundEnd - soundStart));
    //             _stream->println("ms)");
    //         }
    //         else
    //         {
    //             soundStart = lastSound;
    //             soundEnd = 0;
    //             withinSound = true;
    //         }
    //     }
    // }
}