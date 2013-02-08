#include "unity.h"
#include "../include/problem_0002.h"

void setUp() {
}

void tearDown() {
}

void test_problem_0002_should_sum_to_231_with_max_of_10() {
    TEST_ASSERT_EQUAL_INT(231, problem_0002(10));
}

