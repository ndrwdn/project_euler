#include "unity.h"
#include "../include/problem_0002.h"

void setUp() {
}

void tearDown() {
}

void test_problem_0002_should_sum_to_44_with_max_of_40() {
    TEST_ASSERT_EQUAL_INT(44, problem_0002(40));
}

void test_problem_0002_should_sum_to_4613732_with_max_of_4000000() {
    TEST_ASSERT_EQUAL_INT(4613732, problem_0002(4000000));
}
