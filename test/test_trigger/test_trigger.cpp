#include <unity.h>
#include "SoftwareTrigger.h"

TimeProvider timeProvider;
RuntimeManager runtimeManager(&timeProvider);
SoftwareTrigger trigger(&runtimeManager);

void setUp(void)
{
    timeProvider.set(10);
    runtimeManager.reset();
    trigger.setReading(true);
}

void test_not_triggered_when_false(void)
{
    timeProvider.set(10);
    trigger.setReading(false);
    TEST_ASSERT_FALSE(trigger.triggered());
}

void test_triggered_if_first_time(void)
{
    timeProvider.set(10);
    TEST_ASSERT_TRUE(trigger.triggered());
}

void test_not_triggered_within_threshold(void)
{
    trigger.triggered();
    timeProvider.set(10);
    TEST_ASSERT_FALSE(trigger.triggered());
}

void test_not_triggered_on_threshold(void)
{
    trigger.triggered();
    timeProvider.set(2010);
    TEST_ASSERT_FALSE(trigger.triggered());
}

void test_triggered_outside_threshold(void)
{
    trigger.triggered();
    timeProvider.set(2011);
    TEST_ASSERT_TRUE(trigger.triggered());
}

void test_not_triggered_after_first_trigger(void)
{
    timeProvider.set(2010);
    trigger.triggered();
    timeProvider.set(2030);
    TEST_ASSERT_FALSE(trigger.triggered());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_not_triggered_when_false);
    RUN_TEST(test_triggered_if_first_time);
    RUN_TEST(test_not_triggered_within_threshold);
    RUN_TEST(test_not_triggered_on_threshold);
    RUN_TEST(test_triggered_outside_threshold);
    RUN_TEST(test_not_triggered_after_first_trigger);
    UNITY_END();

    return 0;
}