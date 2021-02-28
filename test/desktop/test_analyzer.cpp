#include <unity.h>
#include "Analyzer.h"

Analyzer analyzer;

void setUp(void)
{
    analyzer.clear();
}

void tearDown(void)
{
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

void test_count_rhythmic_sound_detected_ignored(void)
{
    analyzer.recordSilence();
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSilence();
    analyzer.recordSound(500);
    TEST_ASSERT_EQUAL(2, analyzer.countRhythmicSounds());
}

void test_duration_rhythmic_sound_detected_ignored(void)
{
    analyzer.recordSilence();
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSilence();

    TEST_ASSERT_EQUAL(50, analyzer.durationRhythmicSounds());
}

void test_duration_rhythmic_sound_detected_contiguous(void)
{
    analyzer.recordSilence();
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSilence();
    analyzer.recordSound(250);
    analyzer.recordSound(350);
    analyzer.recordSilence();

    TEST_ASSERT_EQUAL(250, analyzer.durationRhythmicSounds());
}

void test_count_rhythmic_sound_detected_separated(void)
{
    analyzer.recordSilence();
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSilence();
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.recordSilence();

    TEST_ASSERT_EQUAL(2, analyzer.countRhythmicSounds());
}

void test_duration_rhythmic_sound_detected_separated(void)
{
    analyzer.recordSilence();
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSilence();
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.recordSilence();

    TEST_ASSERT_EQUAL(150, analyzer.durationRhythmicSounds());
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
    RUN_TEST(test_count_rhythmic_sound_detected_ignored);
    RUN_TEST(test_duration_rhythmic_sound_detected_ignored);
    RUN_TEST(test_duration_rhythmic_sound_detected_contiguous);
    RUN_TEST(test_count_rhythmic_sound_detected_separated);
    RUN_TEST(test_duration_rhythmic_sound_detected_separated);
    UNITY_END();

    return 0;
}