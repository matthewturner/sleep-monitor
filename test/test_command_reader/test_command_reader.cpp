#include <unity.h>
#include "CommandReader.h"
#include "SoftwareStreamReader.h"

SoftwareStreamReader streamReader;
CommandReader commandReader(&streamReader);

void test_invalid_command(void)
{
    streamReader.setCommand("blah");
    unsigned char command = commandReader.tryReadCommand();
    TEST_ASSERT_EQUAL(NONE, command);
}

void test_non_terminated_command(void)
{
    streamReader.setCommand(">inflate");
    unsigned char command = commandReader.tryReadCommand();
    TEST_ASSERT_EQUAL(NONE, command);
}

void test_inflate_command(void)
{
    streamReader.setCommand(">inflate!");
    unsigned char command = commandReader.tryReadCommand();
    TEST_ASSERT_EQUAL(INFLATE, command);
}

void test_deflate_command(void)
{
    streamReader.setCommand(">deflate!");
    unsigned char command = commandReader.tryReadCommand();
    TEST_ASSERT_EQUAL(DEFLATE, command);
}

void test_stop_command(void)
{
    streamReader.setCommand(">stop!");
    unsigned char command = commandReader.tryReadCommand();
    TEST_ASSERT_EQUAL(STOP, command);
}

void test_embedded_command(void)
{
    streamReader.setCommand("random>inflate!random");
    unsigned char command = commandReader.tryReadCommand();
    TEST_ASSERT_EQUAL(INFLATE, command);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_invalid_command);
    RUN_TEST(test_non_terminated_command);
    RUN_TEST(test_inflate_command);
    RUN_TEST(test_deflate_command);
    RUN_TEST(test_stop_command);
    RUN_TEST(test_embedded_command);
    UNITY_END();

    return 0;
}