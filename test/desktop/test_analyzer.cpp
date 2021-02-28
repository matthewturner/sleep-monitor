#include <unity.h>
#include "Analyzer.h"

Analyzer analyzer;

// void setUp(void) {
// // set stuff up here
// }

void tearDown(void)
{
    analyzer.clear();
}

void test_no_rhythmic_sound_detected_empty(void)
{
    TEST_ASSERT_FALSE(analyzer.rhythmicSoundsDetected());
}

void test_no_rhythmic_sound_detected_silence(void)
{
    analyzer.recordSilence();
    analyzer.recordSilence();
    TEST_ASSERT_FALSE(analyzer.rhythmicSoundsDetected());
}

void test_no_rhythmic_sound_detected_frequency_high(void)
{
    analyzer.recordSound(10);
    analyzer.recordSound(20);
    analyzer.recordSound(30);
    TEST_ASSERT_FALSE(analyzer.rhythmicSoundsDetected());
}

void test_rhythmic_sound_detected_insufficient_samples(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(500);
    analyzer.recordSound(900);
    analyzer.recordSound(1300);
    TEST_ASSERT_FALSE(analyzer.rhythmicSoundsDetected());
}

void test_rhythmic_sound_detected_frequency_low(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(500);
    analyzer.recordSound(900);
    analyzer.recordSound(1300);
    analyzer.recordSound(1700);
    TEST_ASSERT_TRUE(analyzer.rhythmicSoundsDetected());
}

void test_count_rhythmic_sound_detected_frequency_low(void)
{
    analyzer.recordSilence();
    analyzer.recordSound(100);
    analyzer.recordSilence();
    analyzer.recordSound(500);
    analyzer.recordSilence();
    analyzer.recordSound(900);
    analyzer.recordSilence();
    analyzer.recordSound(1300);
    analyzer.recordSilence();
    analyzer.recordSound(1700);
    analyzer.recordSilence();
    TEST_ASSERT_EQUAL(5, analyzer.countRhythmicSounds());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_no_rhythmic_sound_detected_empty);
    RUN_TEST(test_no_rhythmic_sound_detected_silence);
    RUN_TEST(test_no_rhythmic_sound_detected_frequency_high);
    RUN_TEST(test_rhythmic_sound_detected_insufficient_samples);
    RUN_TEST(test_rhythmic_sound_detected_frequency_low);
    RUN_TEST(test_count_rhythmic_sound_detected_frequency_low);
    UNITY_END();

    return 0;
}