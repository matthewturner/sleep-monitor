#include <unity.h>
#include "RuntimeManager.h"

RuntimeManager runtimeManager;

void setUp(void)
{
    runtimeManager.setCurrentTime(100);
    runtimeManager.setMinWaitTime(50);
    runtimeManager.setMaxRuntime(100);
}

void test_run_returns_true_if_never_run(void)
{
    TEST_ASSERT_TRUE(runtimeManager.run());
}

void test_run_returns_false_if_runtime_exceeded(void)
{
    runtimeManager.run();
    runtimeManager.setCurrentTime(101);
    TEST_ASSERT_FALSE(runtimeManager.run());
}

void test_run_returns_true_if_wait_time_exceeded(void)
{
    runtimeManager.run();
    runtimeManager.setCurrentTime(101);
    runtimeManager.run();
    runtimeManager.setCurrentTime(51);
    TEST_ASSERT_TRUE(runtimeManager.run());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_run_returns_true_if_never_run);
    RUN_TEST(test_run_returns_false_if_runtime_exceeded);
    RUN_TEST(test_run_returns_true_if_wait_time_exceeded);
    UNITY_END();

    return 0;
}