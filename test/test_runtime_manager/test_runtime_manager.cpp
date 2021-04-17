#include <unity.h>
#include "RuntimeManager.h"

TimeProvider timeProvider;
RuntimeManager runtimeManager(&timeProvider);

void setUp(void)
{
    runtimeManager.setCurrentTime(1);
    runtimeManager.setMinWaitTime(5);
    runtimeManager.setMaxRuntime(10);
    runtimeManager.reset();
}

void test_run_returns_true_if_never_run(void)
{
    TEST_ASSERT_TRUE(runtimeManager.run());
}

void test_run_returns_false_if_runtime_exceeded(void)
{
    for (int i = 0; i <= 10; i++)
    {
        runtimeManager.setCurrentTime(i + 110);
        TEST_ASSERT_TRUE(runtimeManager.run());
    }
    runtimeManager.setCurrentTime(121);
    TEST_ASSERT_FALSE(runtimeManager.run());
}

void test_run_returns_true_if_wait_time_exceeded(void)
{
    runtimeManager.run();
    runtimeManager.setCurrentTime(6);
    TEST_ASSERT_TRUE(runtimeManager.run());
}

void test_run_returns_false_if_runtime_zero_and_wait_time_not_reached(void)
{
    runtimeManager.setMaxRuntime(0);
    runtimeManager.setMinWaitTime(10);
    TEST_ASSERT_TRUE(runtimeManager.run());
    runtimeManager.setCurrentTime(11);
    TEST_ASSERT_FALSE(runtimeManager.run());
}

void test_run_returns_true_if_runtime_zero_and_wait_time_reached(void)
{
    runtimeManager.setMaxRuntime(0);
    runtimeManager.setMinWaitTime(10);
    TEST_ASSERT_TRUE(runtimeManager.run());
    runtimeManager.setCurrentTime(12);
    TEST_ASSERT_TRUE(runtimeManager.run());
}

void test_wait_from_now_does_not_run_on_next(void)
{
    runtimeManager.setMaxRuntime(0);
    runtimeManager.setMinWaitTime(10);
    TEST_ASSERT_TRUE(runtimeManager.run());
    runtimeManager.setCurrentTime(2);
    runtimeManager.waitFromNow();
    runtimeManager.setCurrentTime(12);
    TEST_ASSERT_FALSE(runtimeManager.run());
}

void test_wait_from_now_runs_on_next(void)
{
    runtimeManager.setMaxRuntime(0);
    runtimeManager.setMinWaitTime(10);
    TEST_ASSERT_TRUE(runtimeManager.run());
    runtimeManager.setCurrentTime(2);
    runtimeManager.waitFromNow();
    runtimeManager.setCurrentTime(13);
    TEST_ASSERT_TRUE(runtimeManager.run());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_run_returns_true_if_never_run);
    RUN_TEST(test_run_returns_false_if_runtime_exceeded);
    RUN_TEST(test_run_returns_true_if_wait_time_exceeded);
    RUN_TEST(test_run_returns_false_if_runtime_zero_and_wait_time_not_reached);
    RUN_TEST(test_run_returns_true_if_runtime_zero_and_wait_time_reached);
    RUN_TEST(test_wait_from_now_does_not_run_on_next);
    RUN_TEST(test_wait_from_now_runs_on_next);
    UNITY_END();

    return 0;
}