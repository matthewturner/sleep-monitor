#include <unity.h>

#include "SampleCompressor.h"

SampleCompressor compressor;

void setUp(void)
{
    compressor.reset();
}

void test_returns_zero_on_first_sample(void)
{
    TEST_ASSERT_EQUAL(0, compressor.compress(false));
}

void test_returns_zero_when_first_sample_is_true(void)
{
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
}

void test_returns_zero_on_same_second_sample(void)
{
    compressor.compress(false);
    TEST_ASSERT_EQUAL(0, compressor.compress(false));
}

void test_returns_previous_length_on_sample_change(void)
{
    compressor.compress(false);
    compressor.compress(false);
    TEST_ASSERT_EQUAL(1, compressor.compress(true));
}

void test_returns_previous_length_on_sample_change_inverted(void)
{
    compressor.compress(true);
    compressor.compress(true);
    TEST_ASSERT_EQUAL(1, compressor.compress(false));
}

void test_complex_series(void)
{
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
    TEST_ASSERT_EQUAL(2, compressor.compress(false));
    TEST_ASSERT_EQUAL(0, compressor.compress(false));
    TEST_ASSERT_EQUAL(0, compressor.compress(false));
    TEST_ASSERT_EQUAL(0, compressor.compress(false));
    TEST_ASSERT_EQUAL(0, compressor.compress(false));
    TEST_ASSERT_EQUAL(4, compressor.compress(true));
}

void test_end_with_nothing(void)
{
    TEST_ASSERT_EQUAL(-1, compressor.end());
    TEST_ASSERT_FALSE(compressor.previous());
    TEST_ASSERT_FALSE(compressor.current());
}

void test_end(void)
{
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
    TEST_ASSERT_EQUAL(2, compressor.end());
}

void test_previous_simple(void)
{
    TEST_ASSERT_EQUAL(0, compressor.compress(false));
    TEST_ASSERT_EQUAL(0, compressor.end());
    TEST_ASSERT_FALSE(compressor.previous());
    TEST_ASSERT_FALSE(compressor.current());
}

void test_previous_complex(void)
{
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
    TEST_ASSERT_EQUAL(0, compressor.compress(true));
    TEST_ASSERT_EQUAL(1, compressor.compress(false));
    TEST_ASSERT_EQUAL(0, compressor.end());
    TEST_ASSERT_TRUE(compressor.previous());
    TEST_ASSERT_FALSE(compressor.current());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_returns_zero_on_first_sample);
    RUN_TEST(test_returns_zero_on_same_second_sample);
    RUN_TEST(test_returns_previous_length_on_sample_change);
    RUN_TEST(test_returns_zero_when_first_sample_is_true);
    RUN_TEST(test_returns_previous_length_on_sample_change_inverted);
    RUN_TEST(test_complex_series);
    RUN_TEST(test_end_with_nothing);
    RUN_TEST(test_end);
    RUN_TEST(test_previous_simple);
    RUN_TEST(test_previous_complex);
    UNITY_END();

    return 0;
}