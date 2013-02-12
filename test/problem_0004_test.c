#include "unity.h"
#include "../include/problem_0004.h"

void setUp() {
}

void tearDown() {
}

void test_problem_0004_should_find_largest_palindrome_9009_for_product_of_two_digit_numbers() {
    TEST_ASSERT_EQUAL_INT(9009, problem_0004(2));
}

void test_problem_0004_should_find_largest_palindrome_906609_for_product_of_three_digit_numbers() {
    TEST_ASSERT_EQUAL_INT(906609, problem_0004(3));
}
