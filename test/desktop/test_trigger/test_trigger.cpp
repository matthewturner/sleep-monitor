#include <unity.h>
#include "Trigger.h"

Trigger trigger;

void setUp(void)
{
    trigger.setLastTime(0);
}

void test_not_triggered_when_false(void)
{
    TEST_ASSERT_FALSE(trigger.triggered(false, 10));
}

void test_not_triggered_within_threshold(void)
{
    TEST_ASSERT_FALSE(trigger.triggered(true, 10));
}

void test_triggered_outside_threshold(void)
{
    TEST_ASSERT_TRUE(trigger.triggered(true, 2010));
}

void test_not_triggered_after_first_trigger(void)
{
    trigger.triggered(true, 2010);
    TEST_ASSERT_FALSE(trigger.triggered(true, 2030));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_not_triggered_when_false);
    RUN_TEST(test_not_triggered_within_threshold);
    RUN_TEST(test_triggered_outside_threshold);
    UNITY_END();

    return 0;
}