#include "unity.h"
#include "../include/problem_0003.h"

void setUp() {
}

void tearDown() {
}

void test_problem_0003_should_find_29_as_largest_prime_factor_of_13195() {
    TEST_ASSERT_EQUAL_INT(29, problem_0003(13195));
}

void test_problem_0003_should_find_6857_as_largest_prime_factor_of_600851475143() {
    TEST_ASSERT_EQUAL_INT(6857, problem_0003(600851475143));
}
