#include "../include/problem_0002.h"
#include "../include/arraylist.h"

arraylist *fibonacci_up_to(int maximum) {
    arraylist *list = create_arraylist();
    add_arraylist(list, 1);
    add_arraylist(list, 2);

    int current_term = 2;
    while (current_term <= maximum) {
        current_term = list->list[list->size - 2] + list->list[list->size - 1];
        add_arraylist(list, current_term);
    }

    return list;
}

int problem_0002(int maximum) {
    arraylist *fibs = fibonacci_up_to(maximum);

    int sum = 0;
    int i;

    for (i = 0; i < fibs->size; i++) {
        int fib = fibs->list[i];
        if (fib % 2 == 0) {
            sum += fib;
        }
    }

    destroy_arraylist(fibs);

    return sum;
}
