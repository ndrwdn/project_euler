#include "unity.h"
#include "../include/problem_0001.h"

void setUp() {
}

void tearDown() {
}

void test_problem_0001_should_sum_to_23_with_max_of_10() {
    TEST_ASSERT_EQUAL_INT(23, problem_0001(10));
}

void test_problem_0001_should_sum_to_233168_with_max_of_1000() {
    TEST_ASSERT_EQUAL_INT(233168, problem_0001(1000));
}
