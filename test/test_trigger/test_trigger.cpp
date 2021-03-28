#include <unity.h>
#include "SoftwareTrigger.h"

SoftwareTrigger trigger;

void setUp(void)
{
    trigger.setLastTime(0);
    trigger.setReading(true);
}

void test_not_triggered_when_false(void)
{
    trigger.setReading(false);
    TEST_ASSERT_FALSE(trigger.triggered(10));
}

void test_not_triggered_within_threshold(void)
{
    TEST_ASSERT_FALSE(trigger.triggered(10));
}

void test_triggered_outside_threshold(void)
{
    TEST_ASSERT_TRUE(trigger.triggered(2010));
}

void test_not_triggered_after_first_trigger(void)
{
    trigger.triggered(2010);
    TEST_ASSERT_FALSE(trigger.triggered(2030));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_not_triggered_when_false);
    RUN_TEST(test_not_triggered_within_threshold);
    RUN_TEST(test_triggered_outside_threshold);
    RUN_TEST(test_not_triggered_after_first_trigger);
    UNITY_END();

    return 0;
}