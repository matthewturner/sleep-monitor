#include <unity.h>
#include "SoftwareEndStop.h"

SoftwareEndStop endStop;

void setUp(void)
{
    endStop.setReading(true);
}

void test_not_triggered_when_false(void)
{
    endStop.setReading(false);
    TEST_ASSERT_FALSE(endStop.reached());
}

void test_triggered_when_true(void)
{
    TEST_ASSERT_TRUE(endStop.reached());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_not_triggered_when_false);
    RUN_TEST(test_triggered_when_true);
    UNITY_END();

    return 0;
}