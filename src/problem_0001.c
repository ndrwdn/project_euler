#include "../include/problem_0001.h"

int isMultipleOfThreeOrFive(int n) {
    if (n % 3 == 0 || n % 5 == 0) {
        return 1;
    }
    return 0;
}

int problem_0001(int upper_limit) {
    int sum = 0, i;

    for (i = 0; i < upper_limit; i++) {
        if (isMultipleOfThreeOrFive(i)) {
            sum += i;
        }
    }

    return sum;
}
