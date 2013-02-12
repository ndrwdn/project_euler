#include "../include/problem_0004.h"
#include <math.h>

int get_maximum_for_number_of_digits(int number_of_digits) {
    return pow(10, number_of_digits) - 1;
}

int get_minimum_for_number_of_digits(int number_of_digits) {
    return pow(10, number_of_digits - 1);
}

int reverse_number(int n) {
    int reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n = n / 10;
    }
    return reversed;
}

int is_palindrome(int n) {
    int number_of_digits = (int) log10(n) + 1;
    int half_number_of_digits = number_of_digits / 2;
    int power = (int) pow(10, half_number_of_digits);
    int first = n / power;
    int last = n - first * power;

    int first_reversed = reverse_number(first);

    if (first_reversed == last) {
        return 1;
    }

    return 0;
}

int problem_0004(int number_of_digits) {
    int max = get_maximum_for_number_of_digits(number_of_digits);
    int i, j;
    int lower_bound = get_minimum_for_number_of_digits(number_of_digits);
    int last_pal = 0;

    for (i = max; i >= lower_bound; i--) {
        for (j = max; j >= lower_bound; j--) {
            int mult = i * j;
            if (is_palindrome(mult) && mult > last_pal) {
                last_pal = mult;
            }
        }
    }

    return last_pal;
}
