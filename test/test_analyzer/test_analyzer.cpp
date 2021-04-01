#include <unity.h>
#include "Analyzer.h"

Analyzer analyzer;
Summary summary;

void setUp(void)
{
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
    analyzer.recordSound(500);
    analyzer.recordSound(900);
    analyzer.recordSound(1300);
    analyzer.recordSound(1700);
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
    UNITY_END();

    return 0;
}