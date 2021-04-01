#include <unity.h>
#include "Stepper.h"
#include "SoftwareEndStop.h"
#include "SoftwareStepper.h"
#include "Pillow.h"

SoftwareEndStop endStopTop;
SoftwareEndStop endStopBottom;
SoftwareStepper stepper;
Pillow pillow(&endStopTop, &endStopBottom, &stepper);

void setUp(void)
{
    endStopTop.setReading(false);
    endStopBottom.setReading(false);
    stepper.disable();
    stepper.reset();
}

void test_start_sets_intention(void)
{
    pillow.start(INFLATING);
    TEST_ASSERT_EQUAL(INFLATING, pillow.intention());
}

void test_start_sets_speed(void)
{
    pillow.start(INFLATING);
    TEST_ASSERT_EQUAL(500, stepper.currentSpeed());
}

void test_start_enables_stepper(void)
{
    pillow.start(INFLATING);
    TEST_ASSERT_TRUE(stepper.enabled());
}

void test_running(void)
{
    pillow.start(INFLATING);
    TEST_ASSERT_TRUE(pillow.running());
}

void test_stopped(void)
{
    pillow.start(INFLATING);
    TEST_ASSERT_FALSE(pillow.stopped());
}

void test_stop_disables_stepper(void)
{
    pillow.start(INFLATING);
    pillow.stop();
    TEST_ASSERT_FALSE(stepper.enabled());
}

void test_check_does_not_disable(void)
{
    pillow.start(INFLATING);
    pillow.check();
    TEST_ASSERT_TRUE(stepper.enabled());
}

void test_check_disables_when_deflated(void)
{
    pillow.start(DEFLATING);
    endStopTop.setReading(true);
    pillow.check();
    TEST_ASSERT_FALSE(stepper.enabled());
}

void test_check_does_not_disable_when_deflated_but_inflating(void)
{
    pillow.start(INFLATING);
    endStopTop.setReading(true);
    pillow.check();
    TEST_ASSERT_TRUE(stepper.enabled());
}

void test_check_disables_when_inflated(void)
{
    pillow.start(INFLATING);
    endStopBottom.setReading(true);
    pillow.check();
    TEST_ASSERT_FALSE(stepper.enabled());
}

void test_check_does_not_disable_when_inflated_but_deflating(void)
{
    pillow.start(DEFLATING);
    endStopBottom.setReading(true);
    pillow.check();
    TEST_ASSERT_TRUE(stepper.enabled());
}

void test_proceed_calls_run(void)
{
    pillow.proceed();
    TEST_ASSERT_TRUE(stepper.wasRun());
}

void test_reverse_stops_inflating(void)
{
    pillow.start(INFLATING);
    pillow.reverse();
    TEST_ASSERT_TRUE(pillow.stopped());
}

void test_reverse_starts_deflating(void)
{
    pillow.start(INFLATING);
    pillow.reverse();
    pillow.reverse();
    TEST_ASSERT_EQUAL(DEFLATING, pillow.intention());
    TEST_ASSERT_TRUE(pillow.running());
}

void test_reverse_inflates_immediately_when_deflated(void)
{
    endStopTop.setReading(true);
    pillow.reverse();
    TEST_ASSERT_EQUAL(INFLATING, pillow.intention());
}

void test_reverse_deflates_immediately_when_inflated(void)
{
    endStopBottom.setReading(true);
    pillow.reverse();
    TEST_ASSERT_EQUAL(DEFLATING, pillow.intention());
}

void test_try_deflate_starts(void)
{
    TEST_ASSERT_TRUE(pillow.tryDeflate());
    TEST_ASSERT_EQUAL(DEFLATING, pillow.intention());
    TEST_ASSERT_TRUE(pillow.running());
}

void test_try_deflate_does_not_start(void)
{
    endStopTop.setReading(true);
    TEST_ASSERT_FALSE(pillow.tryDeflate());
    TEST_ASSERT_FALSE(pillow.running());
}

void test_try_inflate_starts(void)
{
    TEST_ASSERT_TRUE(pillow.tryInflate());
    TEST_ASSERT_EQUAL(INFLATING, pillow.intention());
    TEST_ASSERT_TRUE(pillow.running());
}

void test_try_inflate_does_not_start(void)
{
    endStopBottom.setReading(true);
    TEST_ASSERT_FALSE(pillow.tryInflate());
    TEST_ASSERT_FALSE(pillow.running());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_start_sets_intention);
    RUN_TEST(test_start_sets_speed);
    RUN_TEST(test_start_enables_stepper);
    RUN_TEST(test_running);
    RUN_TEST(test_stopped);
    RUN_TEST(test_stop_disables_stepper);
    RUN_TEST(test_check_does_not_disable);
    RUN_TEST(test_check_disables_when_deflated);
    RUN_TEST(test_check_does_not_disable_when_deflated_but_inflating);
    RUN_TEST(test_check_disables_when_inflated);
    RUN_TEST(test_check_does_not_disable_when_inflated_but_deflating);
    RUN_TEST(test_proceed_calls_run);
    RUN_TEST(test_reverse_stops_inflating);
    RUN_TEST(test_reverse_starts_deflating);
    RUN_TEST(test_reverse_inflates_immediately_when_deflated);
    RUN_TEST(test_reverse_deflates_immediately_when_inflated);
    RUN_TEST(test_try_deflate_starts);
    RUN_TEST(test_try_deflate_does_not_start);
    RUN_TEST(test_try_inflate_starts);
    RUN_TEST(test_try_inflate_does_not_start);
    UNITY_END();

    return 0;
}