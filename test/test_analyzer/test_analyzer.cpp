#include <unity.h>
#include "Analyzer.h"

Analyzer analyzer;
Summary summary;

void setUp(void)
{
    analyzer.setSoundDurationThreshold(40);
    analyzer.clear();
    summary.AverageDuration = 0;
    summary.Count = 0;
    summary.TotalDuration = 0;
}

void test_no_rhythmic_sound_detected_empty(void)
{
    analyzer.analyze(&summary);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_no_rhythmic_sound_detected_frequency_high(void)
{
    analyzer.recordSound(10);
    analyzer.recordSound(20);
    analyzer.recordSound(30);
    analyzer.analyze(&summary);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_rhythmic_sound_detected_insufficient_samples(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(500);
    analyzer.recordSound(900);
    analyzer.recordSound(1300);
    analyzer.analyze(&summary);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_rhythmic_sound_detected_frequency_low(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(500);
    analyzer.recordSound(550);
    analyzer.recordSound(900);
    analyzer.recordSound(950);
    analyzer.recordSound(1300);
    analyzer.recordSound(1350);
    analyzer.recordSound(1700);
    analyzer.recordSound(1750);
    analyzer.analyze(&summary);
    TEST_ASSERT_TRUE(summary.RhythmDetected);
}

void test_count_rhythmic_sound_detected_frequency_low(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(500);
    analyzer.recordSound(900);
    analyzer.recordSound(1300);
    analyzer.recordSound(1700);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(5, summary.Count);
}

void test_count_rhythmic_sound_detected_ignored(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(500);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(2, summary.Count);
}

void test_duration_rhythmic_sound_detected_ignored(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(50, summary.TotalDuration);
}

void test_duration_rhythmic_sound_detected_contiguous(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(250);
    analyzer.recordSound(350);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(250, summary.TotalDuration);
}

void test_count_rhythmic_sound_detected_separated(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(2, summary.Count);
}

void test_duration_rhythmic_sound_detected_separated(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(150, summary.TotalDuration);
}

void test_analysis_required_false_when_empty(void)
{
    TEST_ASSERT_FALSE(analyzer.analysisRequired(5002));
}

void test_analysis_required_false(void)
{
    analyzer.recordSound(5001);
    TEST_ASSERT_FALSE(analyzer.analysisRequired(5002));
}

void test_analysis_required_true(void)
{
    analyzer.recordSound(5000);
    analyzer.recordSound(25001);
    TEST_ASSERT_TRUE(analyzer.analysisRequired(25002));
}

void test_analysis_required_by_record(void)
{
    analyzer.recordSound(5000);
    analyzer.recordSound(25001);
    TEST_ASSERT_TRUE(analyzer.analysisRequired(25002));
}

void test_analysis_required_after_significant_break(void)
{
    analyzer.recordSound(5000);
    TEST_ASSERT_TRUE(analyzer.analysisRequired(25001));
}

void test_record_silence(void)
{
    analyzer.record(false, 5000);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(0, summary.Count);
}

void test_record_sound(void)
{
    analyzer.record(true, 5000);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(1, summary.Count);
}

void test_only_last_sound_is_stored_single(void)
{
    analyzer.recordSound(100);
    TEST_ASSERT_EQUAL(1, analyzer.count());
}

void test_only_last_sound_is_stored_double(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(200);
    TEST_ASSERT_EQUAL(2, analyzer.count());
}

void test_only_last_sound_is_stored(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(250);
    analyzer.recordSound(350);
    TEST_ASSERT_EQUAL(2, analyzer.count());
}

void test_only_last_sound_is_stored_separated(void)
{
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    TEST_ASSERT_EQUAL(4, analyzer.count());
}

void test_average_duration_threshold_not_met(void)
{
    analyzer.setSoundDurationThreshold(60);
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.analyze(&summary);
    TEST_ASSERT_FALSE(summary.RhythmDetected);
}

void test_average_duration_threshold(void)
{
    analyzer.setSoundDurationThreshold(60);
    analyzer.recordSound(100);
    analyzer.recordSound(150);
    analyzer.recordSound(550);
    analyzer.recordSound(650);
    analyzer.analyze(&summary);
    TEST_ASSERT_EQUAL(75, summary.AverageDuration);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_no_rhythmic_sound_detected_empty);
    RUN_TEST(test_no_rhythmic_sound_detected_frequency_high);
    RUN_TEST(test_rhythmic_sound_detected_insufficient_samples);
    RUN_TEST(test_rhythmic_sound_detected_frequency_low);
    RUN_TEST(test_count_rhythmic_sound_detected_frequency_low);
    RUN_TEST(test_count_rhythmic_sound_detected_ignored);
    RUN_TEST(test_duration_rhythmic_sound_detected_ignored);
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
    RUN_TEST(test_average_duration_threshold);
    UNITY_END();

    return 0;
}