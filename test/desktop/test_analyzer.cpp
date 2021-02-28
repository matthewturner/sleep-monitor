#include <unity.h>
#include "Analyzer.h"

Analyzer analyzer;

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_function_analyzer_analyze_silence(void)
{
    TEST_ASSERT_EQUAL(false, analyzer.analyze());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_function_analyzer_analyze_silence);
    UNITY_END();

    return 0;
}