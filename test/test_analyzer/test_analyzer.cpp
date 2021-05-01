#include <unity.h>
#include "Analyzer.h"

TimeProvider timeProvider;
Analyzer analyzer(&timeProvider);
Summary summary;

void setUp(void)
{
    analyzer.setSampleThreshold(DEFAULT_MIN_SAMPLE_THRESHOLD, DEFAULT_MAX_SAMPLE_THRESHOLD);
    analyzer.setSoundDurationThreshold(40, 200);
    analyzer.setSilenceDurationThreshold(340, 800);
    analyzer.setMaxSoundStandardDeviation(10000);
    analyzer.setMaxSilenceStandardDeviation(10000);
    analyzer.clear();
}

void test_summary_cleared(void)
{
    summary.AverageSoundDuration = 99;
    summary.AverageSilenceDuration = 99;
    summary.SoundCount = 99;
    summary.TotalSoundDuration = 99;
    summary.TotalSilenceDuration = 99;
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(0, summary.SoundCount);
    TEST_ASSERT_EQUAL(0, summary.AverageSilenceDuration);
    TEST_ASSERT_EQUAL(0, summary.AverageSoundDuration);
    TEST_ASSERT_EQUAL(0, summary.TotalSilenceDuration);
    TEST_ASSERT_EQUAL(0, summary.TotalSoundDuration);
}

void test_no_rhythmic_sound_detected_empty(void)
{
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(INSUFFICIENT_SAMPLE_COUNT, summary.Result);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_no_rhythmic_sound_detected_frequency_high(void)
{
    analyzer.recordSound(10);
    analyzer.recordSound(20);
    analyzer.recordSound(30);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(INSUFFICIENT_SAMPLE_COUNT, summary.Result);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_rhythmic_sound_detected_insufficient_samples(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(500);
    analyzer.recordSound(900);
    analyzer.recordSound(1300);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(INSUFFICIENT_SAMPLE_COUNT, summary.Result);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_rhythmic_sound_detected_frequency_low(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    analyzer.recordSound(500);
    analyzer.recordSound(549);
    analyzer.recordSound(900);
    analyzer.recordSound(949);
    analyzer.recordSound(1300);
    analyzer.recordSound(1349);
    analyzer.recordSound(1700);
    analyzer.recordSound(1749);
    timeProvider.set(1750);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(RHYTHM_DETECTED, summary.Result);
    TEST_ASSERT_TRUE(summary.RhythmDetected);
}

void test_rhythmic_sound_detected_silence_too_low(void)
{
    analyzer.setSilenceDurationThreshold(500, 700);
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    analyzer.recordSound(500);
    analyzer.recordSound(549);
    analyzer.recordSound(900);
    analyzer.recordSound(949);
    analyzer.recordSound(1300);
    analyzer.recordSound(1349);
    analyzer.recordSound(1700);
    analyzer.recordSound(1749);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(INSUFFICIENT_SILENCE_DURATION, summary.Result);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_count_rhythmic_sound_detected_frequency_low(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(500);
    analyzer.recordSound(900);
    analyzer.recordSound(1300);
    analyzer.recordSound(1700);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(5, summary.SoundCount);
}

void test_count_rhythmic_sound_detected_ignored(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(500);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(2, summary.SoundCount);
}

void test_duration_rhythmic_sound_detected_ignored(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(50, summary.TotalSoundDuration);
}

void test_silence_duration_at_end(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    timeProvider.set(300);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(150, summary.TotalSilenceDuration);
}

void test_average_silence_duration_at_end(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    timeProvider.set(300);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(150, summary.AverageSilenceDuration);
}

void test_total_silence_duration_ignore_first(void)
{
    analyzer.recordSound(5000);
    analyzer.recordSound(5149);
    timeProvider.set(5150);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(0, summary.TotalSilenceDuration);
}

void test_duration_rhythmic_sound_detected_contiguous(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(250);
    analyzer.recordSound(349);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(250, summary.TotalSoundDuration);
}

void test_count_rhythmic_sound_detected_separated(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(2, summary.SoundCount);
}

void test_duration_rhythmic_sound_detected_separated(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    analyzer.recordSound(550);
    analyzer.recordSound(649);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(150, summary.TotalSoundDuration);
}

void test_analysis_required_false_when_empty(void)
{
    timeProvider.set(5002);
    TEST_ASSERT_FALSE(analyzer.analysisRequired());
}

void test_analysis_required_false(void)
{
    analyzer.recordSound(5001);
    timeProvider.set(5002);
    TEST_ASSERT_FALSE(analyzer.analysisRequired());
}

void test_analysis_required_true(void)
{
    analyzer.recordSound(5000);
    analyzer.recordSound(25001);
    timeProvider.set(25002);
    TEST_ASSERT_TRUE(analyzer.analysisRequired());
}

void test_analysis_required_by_record(void)
{
    analyzer.recordSound(5000);
    analyzer.recordSound(25001);
    timeProvider.set(25002);
    TEST_ASSERT_TRUE(analyzer.analysisRequired());
}

void test_analysis_required_after_significant_break(void)
{
    analyzer.recordSound(5000);
    timeProvider.set(25001);
    TEST_ASSERT_TRUE(analyzer.analysisRequired());
}

void test_record_silence(void)
{
    timeProvider.set(5000);
    analyzer.record(false);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(0, summary.SoundCount);
}

void test_record_sound(void)
{
    timeProvider.set(5000);
    analyzer.record(true);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(1, summary.SoundCount);
}

void test_only_last_sound_is_stored_single(void)
{
    analyzer.recordSound(100);
    TEST_ASSERT_EQUAL(1, analyzer.count());
}

void test_only_last_sound_is_stored_double(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(199);
    TEST_ASSERT_EQUAL(10, analyzer.count());
}

void test_only_last_sound_is_stored(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(250);
    analyzer.recordSound(349);
    TEST_ASSERT_EQUAL(25, analyzer.count());
}

void test_only_last_sound_is_stored_separated(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(649);
    TEST_ASSERT_EQUAL(55, analyzer.count());
}

void test_average_duration_threshold_not_met(void)
{
    analyzer.setSampleThreshold(1, 100);
    analyzer.setSoundDurationThreshold(60, 500);
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    analyzer.recordSound(550);
    analyzer.recordSound(599);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(INSUFFICIENT_SOUND_DURATION, summary.Result);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_average_sound_duration(void)
{
    analyzer.setSoundDurationThreshold(60, 500);
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    analyzer.recordSound(550);
    analyzer.recordSound(649);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(75, summary.AverageSoundDuration);
}

void test_average_silence_duration_single(void)
{
    analyzer.setSoundDurationThreshold(60, 500);
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    analyzer.recordSound(550);
    analyzer.recordSound(649);
    timeProvider.set(650);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(400, summary.AverageSilenceDuration);
}

void test_average_silence_duration_multiple(void)
{
    analyzer.setSoundDurationThreshold(60, 500);
    analyzer.recordSound(100);
    analyzer.recordSound(149);
    analyzer.recordSound(550);
    analyzer.recordSound(649);
    analyzer.recordSound(1150);
    analyzer.recordSound(1249);
    timeProvider.set(1250);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(450, summary.AverageSilenceDuration);
}

void test_average_silence_duration_threshold_exceeded(void)
{
    analyzer.setSampleThreshold(1, 100);
    analyzer.setSilenceDurationThreshold(200, 300);
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(EXCESSIVE_SILENCE_DURATION, summary.Result);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_average_sound_duration_threshold_exceeded(void)
{
    analyzer.setSampleThreshold(1, 100);
    analyzer.setSoundDurationThreshold(1, 49);
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(600);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(EXCESSIVE_SOUND_DURATION, summary.Result);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_display_is_initialized(void)
{
    analyzer.analyze(&summary);

    for (int i = 0; i < DISPLAY_LENGTH; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
}

void test_display_shows_instant_sound(void)
{
    analyzer.recordSound(100);
    analyzer.analyze(&summary);

    TEST_ASSERT_EQUAL('|', summary.Display[0]);
    for (int i = 1; i < DISPLAY_LENGTH; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
}

void test_display_shows_single_sound(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(199);
    analyzer.analyze(&summary);

    TEST_ASSERT_EQUAL('|', summary.Display[0]);
    for (int i = 1; i < DISPLAY_LENGTH; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
}

void test_display_shows_single_long_sound(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(200);
    analyzer.recordSound(299);
    analyzer.analyze(&summary);

    TEST_ASSERT_EQUAL('|', summary.Display[0]);
    TEST_ASSERT_EQUAL('|', summary.Display[1]);
    for (int i = 2; i < DISPLAY_LENGTH; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
}

void test_display_shows_single_long_sound_beyond_slice_duration(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(201);
    analyzer.analyze(&summary);

    TEST_ASSERT_EQUAL('|', summary.Display[0]);
    TEST_ASSERT_EQUAL('|', summary.Display[1]);
    for (int i = 2; i < DISPLAY_LENGTH; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
}

void test_display_shows_multiple_long_sound_beyond_slice_duration(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(200);
    analyzer.recordSound(401);
    analyzer.recordSound(601);
    analyzer.analyze(&summary);

    TEST_ASSERT_EQUAL('|', summary.Display[0]);
    TEST_ASSERT_EQUAL('|', summary.Display[1]);
    TEST_ASSERT_EQUAL('_', summary.Display[2]);
    TEST_ASSERT_EQUAL('|', summary.Display[3]);
    TEST_ASSERT_EQUAL('|', summary.Display[4]);

    for (int i = 5; i < DISPLAY_LENGTH; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
}

void test_display_shows_multiple_long_sound_realistic(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(301);
    analyzer.recordSound(2100);
    analyzer.recordSound(2200);
    analyzer.recordSound(4150);
    analyzer.recordSound(4380);
    analyzer.analyze(&summary);

    TEST_ASSERT_EQUAL('|', summary.Display[0]);
    TEST_ASSERT_EQUAL('|', summary.Display[1]);
    for (int i = 2; i <= 9; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
    TEST_ASSERT_EQUAL('|', summary.Display[10]);
    for (int i = 11; i <= 19; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
    TEST_ASSERT_EQUAL('|', summary.Display[20]);
    TEST_ASSERT_EQUAL('|', summary.Display[21]);
    for (int i = 22; i < DISPLAY_LENGTH; i++)
    {
        TEST_ASSERT_EQUAL('_', summary.Display[i]);
    }
}

void test_sound_zero_standard_deviation(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(600);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(50, summary.AverageSoundDuration);
    TEST_ASSERT_EQUAL(0, summary.SoundStandardDeviation);
}

void test_basic_sound_standard_deviation(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(75, summary.AverageSoundDuration);
    TEST_ASSERT_EQUAL(35, summary.SoundStandardDeviation);
}

void test_multiple_sound_standard_deviation(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.recordSound(1000);
    analyzer.recordSound(1050);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(66, summary.AverageSoundDuration);
    TEST_ASSERT_EQUAL(28, summary.SoundStandardDeviation);
}

void test_excessive_sound_standard_deviation(void)
{
    analyzer.setSampleThreshold(1, 100);
    analyzer.setSoundDurationThreshold(1, 1000);
    analyzer.setSilenceDurationThreshold(1, 1000);
    analyzer.setMaxSoundStandardDeviation(27);
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.recordSound(1000);
    analyzer.recordSound(1050);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(66, summary.AverageSoundDuration);
    TEST_ASSERT_EQUAL(28, summary.SoundStandardDeviation);
    TEST_ASSERT_EQUAL(EXCESSIVE_SOUND_DEVIATION, summary.Result);
}

void test_silence_zero_standard_deviation(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    timeProvider.set(1050);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(400, summary.AverageSilenceDuration);
    TEST_ASSERT_EQUAL(0, summary.SilenceStandardDeviation);
}

void test_basic_silence_standard_deviation(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    timeProvider.set(1000);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(375, summary.AverageSilenceDuration);
    TEST_ASSERT_EQUAL(35, summary.SilenceStandardDeviation);
}

void test_multiple_silence_standard_deviation(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.recordSound(1000);
    analyzer.recordSound(1050);
    timeProvider.set(1150);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(283, summary.AverageSilenceDuration);
    TEST_ASSERT_EQUAL(160, summary.SilenceStandardDeviation);
}

void test_silence_standard_deviation_exceeded(void)
{
    analyzer.setSampleThreshold(1, 100);
    analyzer.setSilenceDurationThreshold(1, 1000);
    analyzer.setMaxSilenceStandardDeviation(150);
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.recordSound(1000);
    analyzer.recordSound(1050);
    timeProvider.set(1150);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(283, summary.AverageSilenceDuration);
    TEST_ASSERT_EQUAL(160, summary.SilenceStandardDeviation);
    TEST_ASSERT_EQUAL(EXCESSIVE_SILENCE_DEVIATION, summary.Result);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_summary_cleared);
    RUN_TEST(test_no_rhythmic_sound_detected_empty);
    RUN_TEST(test_no_rhythmic_sound_detected_frequency_high);
    RUN_TEST(test_rhythmic_sound_detected_insufficient_samples);
    RUN_TEST(test_rhythmic_sound_detected_frequency_low);
    RUN_TEST(test_rhythmic_sound_detected_silence_too_low);
    RUN_TEST(test_count_rhythmic_sound_detected_frequency_low);
    RUN_TEST(test_count_rhythmic_sound_detected_ignored);
    RUN_TEST(test_duration_rhythmic_sound_detected_ignored);
    RUN_TEST(test_silence_duration_at_end);
    RUN_TEST(test_average_silence_duration_at_end);
    RUN_TEST(test_total_silence_duration_ignore_first);
    RUN_TEST(test_duration_rhythmic_sound_detected_contiguous);
    RUN_TEST(test_count_rhythmic_sound_detected_separated);
    RUN_TEST(test_duration_rhythmic_sound_detected_separated);
    RUN_TEST(test_analysis_required_false_when_empty);
    RUN_TEST(test_analysis_required_false);
    RUN_TEST(test_analysis_required_true);
    RUN_TEST(test_analysis_required_by_record);
    RUN_TEST(test_analysis_required_after_significant_break);
    RUN_TEST(test_record_silence);
    RUN_TEST(test_record_sound);
    RUN_TEST(test_only_last_sound_is_stored_single);
    RUN_TEST(test_only_last_sound_is_stored_double);
    RUN_TEST(test_only_last_sound_is_stored);
    RUN_TEST(test_only_last_sound_is_stored_separated);
    RUN_TEST(test_average_duration_threshold_not_met);
    RUN_TEST(test_average_sound_duration);
    RUN_TEST(test_average_silence_duration_single);
    RUN_TEST(test_average_silence_duration_multiple);
    RUN_TEST(test_average_silence_duration_threshold_exceeded);
    RUN_TEST(test_average_sound_duration_threshold_exceeded);
    RUN_TEST(test_display_is_initialized);
    RUN_TEST(test_display_shows_instant_sound);
    RUN_TEST(test_display_shows_single_sound);
    RUN_TEST(test_display_shows_single_long_sound);
    RUN_TEST(test_display_shows_single_long_sound_beyond_slice_duration);
    RUN_TEST(test_display_shows_multiple_long_sound_beyond_slice_duration);
    RUN_TEST(test_display_shows_multiple_long_sound_realistic);
    RUN_TEST(test_sound_zero_standard_deviation);
    RUN_TEST(test_basic_sound_standard_deviation);
    RUN_TEST(test_multiple_sound_standard_deviation);
    RUN_TEST(test_excessive_sound_standard_deviation);
    RUN_TEST(test_silence_zero_standard_deviation);
    RUN_TEST(test_basic_silence_standard_deviation);
    RUN_TEST(test_multiple_silence_standard_deviation);
    RUN_TEST(test_silence_standard_deviation_exceeded);
    UNITY_END();

    return 0;
}