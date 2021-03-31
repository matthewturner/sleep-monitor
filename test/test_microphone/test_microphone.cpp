#include <unity.h>
#include "SoftwareMicrophone.h"

SoftwareMicrophone microphone;

void setUp(void)
{
    microphone.setReading(true);
}

void test_not_triggered_when_false(void)
{
    microphone.setReading(false);
    TEST_ASSERT_FALSE(microphone.soundDetected());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_not_triggered_when_false);
    UNITY_END();

    return 0;
}